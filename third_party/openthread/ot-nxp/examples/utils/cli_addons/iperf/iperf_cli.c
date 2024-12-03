/*
 *  Copyright (c) 2023, The OpenThread Authors.
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *  1. Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *  3. Neither the name of the copyright holder nor the
 *     names of its contributors may be used to endorse or promote products
 *     derived from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 *  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 */

/* -------------------------------------------------------------------------- */
/*                                  Includes                                  */
/* -------------------------------------------------------------------------- */

#include <assert.h>
#include <errno.h>
#include <string.h>

#include "fsl_common.h"

#include <openthread-core-config.h>
#include <openthread/cli.h>
#include <openthread/error.h>
#include <openthread/instance.h>

#include "app_ot.h"
#include "iperf_cli.h"
#include "lwiperf.h"
#include "ot_lwip.h"
#include "lwip/inet.h"
#include "lwip/tcpip.h"

#include "FreeRTOS.h"
#include "timers.h"

/* -------------------------------------------------------------------------- */
/*                             Private definitions                            */
/* -------------------------------------------------------------------------- */

#ifndef IPERF_UDP_CLIENT_RATE
#define IPERF_UDP_CLIENT_RATE (1 * 1024) /* 1 Kbit/s */
#endif

#ifndef IPERF_CLIENT_AMOUNT
#define IPERF_CLIENT_AMOUNT (-1000) /* 10 seconds */
#endif

#define IPERF_UDP_DEFAULT_FACTOR 250

/* -------------------------------------------------------------------------- */
/*                          Private type definitions                          */
/* -------------------------------------------------------------------------- */

struct iperf_test_context
{
    bool                     server_mode;
    bool                     tcp;
    enum lwiperf_client_type client_type;
    void                    *iperf_session;
};

/* -------------------------------------------------------------------------- */
/*                             Private prototypes                             */
/* -------------------------------------------------------------------------- */

/*!
 * @brief FreeRTOS timer callback. Posts UDP polling on tcpip_thread.
 */
static void timer_poll_udp_client(TimerHandle_t timer);

/*!
 * @brief Invokes UDP polling, to be run on tcpip_thread.
 */
static void poll_udp_client(void *arg);

/*!
 * @brief Display iperf cli usage
 *
 */
static void display_iperf_usage(void);

/*!
 * @brief Function to abort iperf test.
 */
static void iperf_test_abort(void *arg);

/*!
 * @brief Function to start iperf test.
 */
static void iperf_test_start(void *arg);

/*!
 * \brief Prints test reports
 *
 * \param[in] arg
 * \param[in] report_type
 * \param[in] local_addr
 * \param[in] local_port
 * \param[in] remote_addr
 * \param[in] remote_port
 * \param[in] bytes_transferred
 * \param[in] ms_duration
 * \param[in] bandwidth_kbitpsec
 */
static void lwiperf_report(void                    *arg,
                           enum lwiperf_report_type report_type,
                           const ip_addr_t         *local_addr,
                           uint16_t                 local_port,
                           const ip_addr_t         *remote_addr,
                           uint16_t                 remote_port,
                           uint64_t                 bytes_transferred,
                           uint32_t                 ms_duration,
                           uint32_t                 bandwidth_kbitpsec);

int get_uint(const char *arg, unsigned int *dest, unsigned int len);

static void TESTAbort(void);

static void TCPServer(void);

static void TCPClient(void);

static void TCPClientDual(void);

static void TCPClientTradeOff(void);

static void UDPServer(void);

static void UDPServerDual(void);

static void UDPClient(void);

static void UDPClientDual(void);

static void UDPClientTradeOff(void);

#ifdef DISABLE_TCPIP_INIT
/* Disable Power Manager tickless IDLE when we are running iperf */
extern void APP_SetTicklessIdle(bool enable);

static bool iperf_need_enable_tickless_idle(void *arg, enum lwiperf_report_type report_type);

static void iperf_disable_tickless_hook(bool disable);
#endif

/* -------------------------------------------------------------------------- */
/*                               Private memory                               */
/* -------------------------------------------------------------------------- */

static TimerHandle_t             iperfTimer = NULL;
static ip_addr_t                 server_address;
static ip_addr_t                 bind_address;
static struct iperf_test_context ctx;
static int                       amount          = IPERF_CLIENT_AMOUNT;
static unsigned int              udp_rate_factor = IPERF_UDP_DEFAULT_FACTOR;
static unsigned int              buffer_len      = 0;
static unsigned int              port            = LWIPERF_TCP_PORT_DEFAULT;

/* Report state => string */
const char *report_type_str[] = {
    "TCP_DONE_SERVER (RX)",        /* LWIPERF_TCP_DONE_SERVER,*/
    "TCP_DONE_CLIENT (TX)",        /* LWIPERF_TCP_DONE_CLIENT,*/
    "TCP_ABORTED_LOCAL",           /* LWIPERF_TCP_ABORTED_LOCAL, */
    "TCP_ABORTED_LOCAL_DATAERROR", /* LWIPERF_TCP_ABORTED_LOCAL_DATAERROR, */
    "TCP_ABORTED_LOCAL_TXERROR",   /* LWIPERF_TCP_ABORTED_LOCAL_TXERROR, */
    "TCP_ABORTED_REMOTE",          /* LWIPERF_TCP_ABORTED_REMOTE, */
    "UDP_DONE_SERVER (RX)",        /* LWIPERF_UDP_DONE_SERVER, */
    "UDP_DONE_CLIENT (TX)",        /* LWIPERF_UDP_DONE_CLIENT, */
    "UDP_ABORTED_LOCAL",           /* LWIPERF_UDP_ABORTED_LOCAL, */
    "UDP_ABORTED_LOCAL_DATAERROR", /* LWIPERF_UDP_ABORTED_LOCAL_DATAERROR, */
    "UDP_ABORTED_LOCAL_TXERROR",   /* LWIPERF_UDP_ABORTED_LOCAL_TXERROR, */
    "UDP_ABORTED_REMOTE",          /* LWIPERF_UDP_ABORTED_REMOTE, */
};

/* -------------------------------------------------------------------------- */
/*                              Public functions                              */
/* -------------------------------------------------------------------------- */

void otAppCliIperfCliInit(otInstance *aInstance)
{
    otPlatLwipInit(aInstance, appOtLockOtTask);
    otPlatLwipAddThreadInterface();

    (void)memset(&ctx, 0, sizeof(struct iperf_test_context));

    iperfTimer = xTimerCreate("UDP Poll Timer", 1 / portTICK_PERIOD_MS, pdTRUE, (void *)0, timer_poll_udp_client);
    assert(iperfTimer != NULL);

    if (xTimerStart(iperfTimer, 0) != pdPASS)
    {
        assert(0);
    }
}

otError ProcessIperf(void *aContext, uint8_t aArgsLength, char *aArgs[])
{
    int     arg               = 0;
    char    ip_addr[128]      = {0};
    char    ip_addr_bind[128] = {0};
    otError status            = OT_ERROR_NONE;

    struct
    {
        unsigned help : 1;
        unsigned udp : 1;
        unsigned bind : 1;
        unsigned bhost : 1;
        unsigned abort : 1;
        unsigned server : 1;
        unsigned client : 1;
        unsigned chost : 1;
        unsigned dual : 1;
        unsigned tradeoff : 1;
        unsigned time : 1;
        unsigned dserver : 1;
        unsigned buflen : 1;
        unsigned port : 1;
    } info;

    amount     = IPERF_CLIENT_AMOUNT;
    buffer_len = 0;
    port       = LWIPERF_TCP_PORT_DEFAULT;

    (void)memset(&info, 0, sizeof(info));

    do
    {
        if (aArgsLength == 0)
        {
            display_iperf_usage();
            break;
        }

        /* Reset udp rate factor */
        udp_rate_factor = IPERF_UDP_DEFAULT_FACTOR;

        /* Parse all args and configure the test accordingly */
        do
        {
            if (!strcmp("-h", aArgs[arg]))
            {
                display_iperf_usage();
                info.help = 1;
                break;
            }
            else if (!strcmp("-u", aArgs[arg]))
            {
                arg += 1;
                info.udp = 1;
            }
            else if (!strcmp("-a", aArgs[arg]))
            {
                arg += 1;
                info.abort = 1;
            }
            else if (!strcmp("-s", aArgs[arg]))
            {
                arg += 1;
                info.server = 1;
            }
            else if (!strcmp("-c", aArgs[arg]))
            {
                arg += 1;
                info.client = 1;

                if (aArgs[arg] != NULL)
                {
                    if (strlen(aArgs[arg]) < sizeof(ip_addr))
                    {
                        strncpy(ip_addr, aArgs[arg], strlen(aArgs[arg]));

                        arg += 1;
                        info.chost = 1;
                    }
                    else
                    {
                        otCliOutputFormat("Invalid ip addr, length exceeded max ip_addr bytes\r\n");
                        status = OT_ERROR_INVALID_ARGS;
                        break;
                    }
                }
            }
            else if (!strcmp("-B", aArgs[arg]))
            {
                arg += 1;
                info.bind = 1;

                if (aArgs[arg] != NULL)
                {
                    if (strlen(aArgs[arg]) < sizeof(ip_addr_bind))
                    {
                        strncpy(ip_addr_bind, aArgs[arg], strlen(aArgs[arg]));
                        arg += 1;
                    }
                    else
                    {
                        otCliOutputFormat("Invalid bind ip addr, length exceeded max ip_addr bytes\r\n");
                        status = OT_ERROR_INVALID_ARGS;
                        break;
                    }
                }
            }
            else if (!strcmp("-t", aArgs[arg]))
            {
                arg += 1;
                info.time = 1;
                errno     = 0;
                amount    = -(100 * strtoul(aArgs[arg], NULL, 10));
                if (errno != 0)
                {
                    otCliOutputFormat("Invalid time, error during strtoul errno:%d\r\n", errno);
                    status = OT_ERROR_INVALID_ARGS;
                    break;
                }
                arg += 1;
            }
            else if (!strcmp("-d", aArgs[arg]))
            {
                arg += 1;
                info.dual = 1;
            }
            else if (!strcmp("-r", aArgs[arg]))
            {
                arg += 1;
                info.tradeoff = 1;
            }
            else if (!strcmp("-b", aArgs[arg]))
            {
                if (arg + 1 >= aArgsLength || get_uint(aArgs[arg + 1], &udp_rate_factor, strlen(aArgs[arg + 1])))
                {
                    otCliOutputFormat("Error: invalid bandwidth argument\r\n");
                    status = OT_ERROR_INVALID_ARGS;
                    break;
                }
                arg += 2;
            }
            else if (!strcmp("-D", aArgs[arg]))
            {
                arg += 1;
                info.dserver = 1;
            }
            else if (!strcmp("-l", aArgs[arg]))
            {
                if (arg + 1 >= aArgsLength || get_uint(aArgs[arg + 1], &buffer_len, strlen(aArgs[arg + 1])))
                {
                    otCliOutputFormat("Error: invalid length argument\r\n");
                    status = OT_ERROR_INVALID_ARGS;
                    break;
                }

                if (buffer_len == 0)
                {
                    otCliOutputFormat("Error: invalid length argument\r\n");
                    status = OT_ERROR_INVALID_ARGS;
                    break;
                }
                arg += 2;
                info.buflen = 1;
            }
            else if (!strcmp("-p", aArgs[arg]))
            {
                if (arg + 1 >= aArgsLength || get_uint(aArgs[arg + 1], &port, strlen(aArgs[arg + 1])))
                {
                    otCliOutputFormat("Error: invalid port argument\r\n");
                    status = OT_ERROR_INVALID_ARGS;
                    break;
                }

                if (port == 0)
                {
                    otCliOutputFormat("Error: invalid port argument\r\n");
                    status = OT_ERROR_INVALID_ARGS;
                    break;
                }
                arg += 2;
                info.port = 1;
            }
            else
            {
                display_iperf_usage();
                status = OT_ERROR_INVALID_ARGS;
                break;
            }
        } while (arg < aArgsLength);

        if (status != OT_ERROR_NONE)
        {
            break;
        }

        inet6_aton(ip_addr, ip_2_ip6(&server_address));
        server_address.type = IPADDR_TYPE_V6;

        if (info.bind)
        {
            inet6_aton(ip_addr_bind, ip_2_ip6(&bind_address));
            bind_address.type = IPADDR_TYPE_V6;
            if (IP_IS_V6(&bind_address) != 0)
            {
                info.bhost = 1;
            }
        }

        if (!info.help && ((!info.abort && !info.server && !info.client) || (info.client && !info.chost) ||
                           (info.server && info.client) || ((info.dual || info.tradeoff) && !info.client) ||
                           (info.dual && info.tradeoff) || (info.dserver && (!info.server || !info.udp)) ||
                           (info.client && (!info.bind || !info.bhost))))
        {
            otCliOutputFormat("Incorrect usage\r\n");
            if (info.client && (!info.bind || !info.bhost))
            {
                otCliOutputFormat("IPv6: For client please specify local interface ip address using -B option\r\n");
            }
            display_iperf_usage();

            status = OT_ERROR_INVALID_ARGS;
            break;
        }

        if (info.abort != 0U)
        {
            TESTAbort();
        }
        else if (info.server != 0U)
        {
            if (info.udp != 0U)
            {
                if (info.dserver != 0U)
                {
                    UDPServerDual();
                }
                else
                {
                    UDPServer();
                }
            }
            else
            {
                TCPServer();
            }
        }
        else if (info.client != 0U)
        {
            if (info.udp != 0U)
            {
                if (info.dual != 0U)
                {
                    UDPClientDual();
                }
                else if (info.tradeoff != 0U)
                {
                    UDPClientTradeOff();
                }
                else
                {
                    UDPClient();
                }
            }
            else
            {
                if (info.dual != 0U)
                {
                    TCPClientDual();
                }
                else if (info.tradeoff != 0U)
                {
                    TCPClientTradeOff();
                }
                else
                {
                    TCPClient();
                }
            }
        }
        else
        { /* Do Nothing */
        }

    } while (false);

    return status;
}

/* -------------------------------------------------------------------------- */
/*                              Private functions                             */
/* -------------------------------------------------------------------------- */

static void timer_poll_udp_client(TimerHandle_t timer)
{
    (void)timer;

    tcpip_try_callback(poll_udp_client, NULL);
}

static void poll_udp_client(void *arg)
{
    (void)arg;

    lwiperf_poll_udp_client();
}

static void display_iperf_usage(void)
{
    otCliOutputFormat("Usage:\r\n");
    otCliOutputFormat("\tiperf [-s|-c <host>|-a] [options]\r\n");
    otCliOutputFormat("\tiperf [-h]\r\n");
    otCliOutputFormat("\r\n");
    otCliOutputFormat("Client/Server:\r\n");
    otCliOutputFormat("\t-u         use UDP rather than TCP\r\n");
    otCliOutputFormat("\t-B <host>  bind to <host>\r\n");
    otCliOutputFormat("\t-a         abort ongoing iperf session\r\n");
    otCliOutputFormat("\t-p         server port to listen on/connect to\r\n");
    otCliOutputFormat("Server specific:\r\n");
    otCliOutputFormat("\t-s         run in server mode\r\n");
    otCliOutputFormat("\t-D         Do a bidirectional UDP test simultaneously (iperf client must use -d)\r\n");
    otCliOutputFormat("Client specific:\r\n");
    otCliOutputFormat("\t-c <host>  run in client mode, connecting to <host>\r\n");
    otCliOutputFormat("\t-d         Do a bidirectional test simultaneously\r\n");
    otCliOutputFormat("\t-r         Do a bidirectional test individually\r\n");
    otCliOutputFormat("\t-t <secs>  time in seconds to transmit for (Default: 10 secs)\r\n");
    otCliOutputFormat("\t-b <kbps>  for UDP, bandwidth to send at in Kpbs (Default: 250Kbps)\r\n");
    otCliOutputFormat("\t-l <bytes> length of buffer in bytes to write (Defaults: TCP: 1220B UDP: 1232B)\r\n");
}

static void iperf_test_abort(void *arg)
{
    struct iperf_test_context *test_ctx = (struct iperf_test_context *)arg;

    if (test_ctx->iperf_session != NULL)
    {
        uint32_t mask = DisableGlobalIRQ();
        lwiperf_abort(test_ctx->iperf_session);
        test_ctx->iperf_session = NULL;
        EnableGlobalIRQ(mask);
    }

    (void)memset(&ctx, 0, sizeof(struct iperf_test_context));
}

static void iperf_test_start(void *arg)
{
    struct iperf_test_context *ctx       = (struct iperf_test_context *)arg;
    struct netif              *netiftmp  = NULL;
    struct netif              *netifbind = NULL;

    if (ctx->iperf_session != NULL)
    {
        otCliOutputFormat("Abort ongoing IPERF session\r\n");
        lwiperf_abort(ctx->iperf_session);
        ctx->iperf_session = NULL;
    }

#ifdef DISABLE_TCPIP_INIT
    /* Disable tickless idle when running iperf test */
    if (ctx->server_mode)
    {
        otCliOutputFormat("Please use iperf -a to close iperf after iperf server mode done\r\n");
    }
    iperf_disable_tickless_hook(true);
#endif

    if (!(ctx->tcp) && ctx->client_type == LWIPERF_DUAL)
    {
        /* Reducing udp Tx timer interval for rx to be served */
        xTimerChangePeriod(iperfTimer, 2 / portTICK_PERIOD_MS, 100);
    }
    else
    {
        /* Returning original timer settings of 1 ms interval*/
        xTimerChangePeriod(iperfTimer, 1 / portTICK_PERIOD_MS, 100);
    }

    if (ctx->server_mode)
    {
        if (ctx->tcp)
        {
            ctx->iperf_session = lwiperf_start_tcp_server(IP6_ADDR_ANY, port, lwiperf_report, ctx);
        }
        else
        {
            ctx->iperf_session = lwiperf_start_udp_server(IP6_ADDR_ANY, port, lwiperf_report, ctx);
        }
    }
    else
    {
        if (ctx->tcp)
        {
            NETIF_FOREACH(netiftmp)
            {
                if (netif_get_ip6_addr_match(netiftmp, ip_2_ip6(&bind_address)) >= 0)
                {
                    netifbind = netiftmp;
                    break;
                }
            }
            if (!netifbind)
            {
                otCliOutputFormat("Could not find corresponding netif by bind_address\r\n");
                lwiperf_abort(ctx->iperf_session);
                ctx->iperf_session = NULL;
                return;
            }
            if (buffer_len == 0)
            {
                /* OPENTHREAD_CONFIG_IP6_MAX_DATAGRAM_LENGTH is the max ipv6 packet size accepted by OpenThread
                 * Substitute 60 bytes for the TCP header size */
                buffer_len = OPENTHREAD_CONFIG_IP6_MAX_DATAGRAM_LENGTH - 60;
            }
            ip6_addr_assign_zone(ip_2_ip6(&server_address), IP6_UNICAST, netifbind);
            ctx->iperf_session = lwiperf_start_tcp_client(&server_address, port, ctx->client_type, amount, buffer_len,
                                                          0, lwiperf_report, ctx);
        }
        else
        {
            if (buffer_len == 0)
            {
                /* OPENTHREAD_CONFIG_IP6_MAX_DATAGRAM_LENGTH is the max ipv6 packet size accepted by OpenThread
                 * Substract 48 bytes for the UDP header size */
                buffer_len = OPENTHREAD_CONFIG_IP6_MAX_DATAGRAM_LENGTH - 48;
            }
            ctx->iperf_session =
                lwiperf_start_udp_client(&bind_address, port, &server_address, port, ctx->client_type, amount,
                                         buffer_len, IPERF_UDP_CLIENT_RATE * udp_rate_factor, 0, lwiperf_report, ctx);
        }
    }

    if (ctx->iperf_session == NULL)
    {
        otCliOutputFormat("IPERF initialization failed!\r\n");
    }
    else
    {
        otCliOutputFormat("IPERF initialization successful\r\n");
    }
}

static void iperf_free_ctx_iperf_session(void *arg, enum lwiperf_report_type report_type)
{
    struct iperf_test_context *ctx = (struct iperf_test_context *)arg;

    if (!ctx || ctx->server_mode ||
        (ctx->client_type == LWIPERF_TRADEOFF &&
         (report_type == LWIPERF_TCP_DONE_CLIENT_TX || report_type == LWIPERF_UDP_DONE_CLIENT_TX)))
        return;

    ctx->iperf_session = NULL;
}

static void lwiperf_report(void                    *arg,
                           enum lwiperf_report_type report_type,
                           const ip_addr_t         *local_addr,
                           uint16_t                 local_port,
                           const ip_addr_t         *remote_addr,
                           uint16_t                 remote_port,
                           uint64_t                 bytes_transferred,
                           uint32_t                 ms_duration,
                           uint32_t                 bandwidth_kbitpsec)
{
    otCliOutputFormat("-------------------------------------------------\r\n");
    if (report_type < (sizeof(report_type_str) / sizeof(report_type_str[0])))
    {
        otCliOutputFormat(" %s \r\n", report_type_str[report_type]);
        if (local_addr && remote_addr)
        {
            otCliOutputFormat(" Local address : %s ", inet6_ntoa(local_addr->u_addr.ip6));
            otCliOutputFormat(" Port %d \r\n", local_port);
            otCliOutputFormat(" Remote address : %s ", inet6_ntoa(remote_addr->u_addr.ip6));
            otCliOutputFormat(" Port %d \r\n", remote_port);
            otCliOutputFormat(" Duration %u ms \r\n", ms_duration);
            otCliOutputFormat(" Bandwidth  %u Kb/s \r\n", bandwidth_kbitpsec);
        }
    }
    else
    {
        otCliOutputFormat(" IPERF Report error\r\n");
    }
    otCliOutputFormat("\r\n");
    iperf_free_ctx_iperf_session(arg, report_type);

#ifdef DISABLE_TCPIP_INIT
    /* Re-enable Tickless Idle */
    if (iperf_need_enable_tickless_idle(arg, report_type))
    {
        iperf_disable_tickless_hook(false);
    }
#endif
}

int get_uint(const char *arg, unsigned int *dest, unsigned int len)
{
    int          i;
    unsigned int val = 0;

    for (i = 0; i < len; i++)
    {
        if (arg[i] < '0' || arg[i] > '9')
            return 1;
        val *= 10;
        val += arg[i] - '0';
    }

    *dest = val;
    return 0;
}

static void TESTAbort(void)
{
    iperf_test_abort((void *)&ctx);
}

static void TCPServer(void)
{
    ctx.server_mode = true;
    ctx.tcp         = true;
    ctx.client_type = LWIPERF_CLIENT;

    tcpip_callback(iperf_test_start, (void *)&ctx);
}

static void TCPClient(void)
{
    ctx.server_mode = false;
    ctx.tcp         = true;
    ctx.client_type = LWIPERF_CLIENT;

    tcpip_callback(iperf_test_start, (void *)&ctx);
}

static void TCPClientDual(void)
{
    ctx.server_mode = false;
    ctx.tcp         = true;
    ctx.client_type = LWIPERF_DUAL;

    tcpip_callback(iperf_test_start, (void *)&ctx);
}

static void TCPClientTradeOff(void)
{
    ctx.server_mode = false;
    ctx.tcp         = true;
    ctx.client_type = LWIPERF_TRADEOFF;

    tcpip_callback(iperf_test_start, (void *)&ctx);
}

static void UDPServer(void)
{
    ctx.server_mode = true;
    ctx.tcp         = false;
    ctx.client_type = LWIPERF_CLIENT;

    tcpip_callback(iperf_test_start, (void *)&ctx);
}

static void UDPServerDual(void)
{
    ctx.server_mode = true;
    ctx.tcp         = false;
    ctx.client_type = LWIPERF_DUAL;

    otCliOutputFormat("Bidirectional UDP test simultaneously as server, please add -d with external iperf client\r\n");
    tcpip_callback(iperf_test_start, (void *)&ctx);
}

static void UDPClient(void)
{
    ctx.server_mode = false;
    ctx.tcp         = false;
    ctx.client_type = LWIPERF_CLIENT;

    tcpip_callback(iperf_test_start, (void *)&ctx);
}

static void UDPClientDual(void)
{
    ctx.server_mode = false;
    ctx.tcp         = false;
    ctx.client_type = LWIPERF_DUAL;

    tcpip_callback(iperf_test_start, (void *)&ctx);
}

static void UDPClientTradeOff(void)
{
    ctx.server_mode = false;
    ctx.tcp         = false;
    ctx.client_type = LWIPERF_TRADEOFF;

    tcpip_callback(iperf_test_start, (void *)&ctx);
}

#ifdef DISABLE_TCPIP_INIT
static void iperf_disable_tickless_hook(bool disable)
{
    static bool disable_tickless = false;

    if (disable == true)
    {
        if (disable_tickless == false)
        {
            APP_SetTicklessIdle(false);
            disable_tickless = true;
        }
    }
    else
    {
        if (disable_tickless == true)
        {
            APP_SetTicklessIdle(true);
            disable_tickless = false;
        }
    }
}

/**
 *  For server mode, we do not re-enable tickless idle in case client runs continuous multiple tests.
 *  For client mode, we re-enable tickless idle except for one case:
 *  We start a bidirectional test individually, first TX and then RX. In this case, for the first TX,
 *  we do not re-enable tickless idle and re-enable it after latter RX is done.
 */
static bool iperf_need_enable_tickless_idle(void *arg, enum lwiperf_report_type report_type)
{
    struct iperf_test_context *ctx = (struct iperf_test_context *)arg;

    if (!ctx)
    {
        return true;
    }

    if (ctx->server_mode)
    {
        return false;
    }
    else if (ctx->client_type == LWIPERF_TRADEOFF &&
             (report_type == LWIPERF_TCP_DONE_CLIENT_TX || report_type == LWIPERF_UDP_DONE_CLIENT_TX))
    {
        return false;
    }

    return true;
}
#endif
