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
#include "fsl_common.h"
#include <openthread/cli.h>
#include "lwip/netif.h"
#include "lwip/stats.h"
#include "lwip/tcpip.h"

/* -------------------------------------------------------------------------- */
/*                             Private definitions                            */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*                             Private prototypes                             */
/* -------------------------------------------------------------------------- */

static otError ProcessIpaddrCmd(void *aContext, uint8_t aArgsLength, char *aArgs[]);
static otError ProcessIplinkCmd(void *aContext, uint8_t aArgsLength, char *aArgs[]);
#if LWIP_STATS && LWIP_STATS_DISPLAY
static otError ProcessStatsCmd(void *aContext, uint8_t aArgsLength, char *aArgs[]);
#endif /* LWIP_STATS && LWIP_STATS_DISPLAY */
static otError ProcessHelpCmd(void *aContext, uint8_t aArgsLength, char *aArgs[]);

/* -------------------------------------------------------------------------- */
/*                               Private memory                               */
/* -------------------------------------------------------------------------- */

static const otCliCommand commands[] = {
    {"ipaddr", ProcessIpaddrCmd},
    {"iplink", ProcessIplinkCmd},
#if LWIP_STATS && LWIP_STATS_DISPLAY
    {"stats", ProcessStatsCmd},
#endif /* LWIP_STATS && LWIP_STATS_DISPLAY */
    {"help", ProcessHelpCmd},
};

/* -------------------------------------------------------------------------- */
/*                              Public functions                              */
/* -------------------------------------------------------------------------- */

otError ProcessLwip(void *aContext, uint8_t aArgsLength, char *aArgs[])
{
    otError error  = OT_ERROR_NONE;
    int     n_cmds = (sizeof(commands) / sizeof(commands[0]));
    int     i      = 0;
    do
    {
        if (aArgsLength == 0)
        {
            error = OT_ERROR_INVALID_ARGS;
            break;
        }

        for (i = 0; i < n_cmds; i++)
        {
            if (!strcmp(commands[i].mName, aArgs[0]))
            {
                error = commands[i].mCommand(aContext, aArgsLength - 1, &aArgs[1]);
                break;
            }
        }
        if (i == n_cmds)
        {
            error = OT_ERROR_INVALID_ARGS;
        }
        break;
    } while (false);

    return error;
}

/* -------------------------------------------------------------------------- */
/*                              Private functions                             */
/* -------------------------------------------------------------------------- */

static struct netif *NetifFromArg(const char *arg)
{
    if (strlen(arg) >= 3)
    {
        const char ch_num = arg[2];
        if ((ch_num >= '0') && (ch_num <= '9'))
        {
            struct netif *n;
            u8_t          if_num = ch_num - '0';

            NETIF_FOREACH(n)
            {
                if (arg[0] == n->name[0] && arg[1] == n->name[1] && if_num == n->num)
                {
                    return n;
                }
            }
        }
    }
    return NULL;
}

static void PrintAddrTime(unsigned long time)
{
    if (ip6_addr_life_isinfinite(time))
    {
        otCliOutputFormat("forever");
    }
    else
    {
        otCliOutputFormat("%us", time);
    }
}

static void PrintIpaddr(const struct netif *n)
{
    otCliOutputFormat("%c%c%u:\r\n", n->name[0], n->name[1], n->num);

    for (int i = 0; i < LWIP_IPV6_NUM_ADDRESSES; i++)
    {
        const u8_t addr_state = netif_ip6_addr_state(n, i);
        if (!ip6_addr_isinvalid(addr_state))
        {
            const char *str_ip = "?";
            str_ip             = ip6addr_ntoa(netif_ip6_addr(n, i));
            otCliOutputFormat(" #%d: %s\r\n      ", i, str_ip);

            if (ip6_addr_istentative(addr_state))
            {
                otCliOutputFormat("tentative");
            }
            else if (ip6_addr_ispreferred(addr_state))
            {
                otCliOutputFormat("preferred");
            }
            else if (ip6_addr_isdeprecated(addr_state))
            {
                otCliOutputFormat("deprecated");
            }
            else if (ip6_addr_isduplicated(addr_state))
            {
                otCliOutputFormat("invalid (DAD failed)");
            }

            if (ip6_addr_life_isstatic(netif_ip6_addr_valid_life(n, i)))
            {
                otCliOutputFormat(" static");
            }
            else
            {
                otCliOutputFormat(" pref_lft: ");
                PrintAddrTime(netif_ip6_addr_pref_life(n, i));
                otCliOutputFormat(" valid_lft: ");
                PrintAddrTime(netif_ip6_addr_valid_life(n, i));
            }
            otCliOutputFormat("\r\n");
        }
    }

#if LWIP_IPV4
    const ip4_addr_t *ip4_addr;
    u32_t             netmask;
    u8_t              netmask_len;
    const char       *str_ip;

    ip4_addr = netif_ip4_addr(n);
    if (!ip4_addr_isany(ip4_addr))
    {
        netmask     = PP_NTOHL(netif_ip4_netmask(n)->addr);
        netmask_len = 0U;
        while ((netmask & 0x80000000U) != 0U)
        {
            netmask_len++;
            netmask <<= 1;
        }

        str_ip = ip4addr_ntoa(ip4_addr);
        otCliOutputFormat("\r\n IPv4 Address: %s/%u\r\n", str_ip, netmask_len);

        ip4_addr = netif_ip4_gw(n);
        if (!ip4_addr_isany(ip4_addr))
        {
            str_ip = ip4addr_ntoa(ip4_addr);
            otCliOutputFormat(" Gateway     : %s\r\n", str_ip);
        }
    }
    otCliOutputFormat("\r\n");
#endif /* LWIP_IPV4 */
}

static otError ProcessIpaddrAddCmd(struct netif *n, uint8_t aArgsLength, char *aArgs[])
{
    if (aArgsLength == 1)
    {
        err_t      add_err;
        ip6_addr_t addr;

        if (!ip6addr_aton(aArgs[0], &addr))
        {
            return OT_ERROR_INVALID_ARGS;
        }

        LOCK_TCPIP_CORE();
        add_err = netif_add_ip6_address(n, &addr, NULL);
        UNLOCK_TCPIP_CORE();

        if (add_err != ERR_OK)
        {
            return OT_ERROR_NOT_CAPABLE;
        }
    }
    else
    {
        return OT_ERROR_INVALID_ARGS;
    }

    return OT_ERROR_NONE;
}

static otError ProcessIpaddrDelCmd(struct netif *n, uint8_t aArgsLength, char *aArgs[])
{
    if (aArgsLength == 1)
    {
        ip6_addr_t addr;
        s8_t       id;
        if (!ip6addr_aton(aArgs[0], &addr))
        {
            return OT_ERROR_INVALID_ARGS;
        }

        id = netif_get_ip6_addr_match(n, &addr);

        if (id < 0)
        {
            return OT_ERROR_NOT_FOUND;
        }

        LOCK_TCPIP_CORE();
        netif_ip6_addr_set_state(n, id, IP6_ADDR_INVALID);
        UNLOCK_TCPIP_CORE();
    }
    else
    {
        return OT_ERROR_INVALID_ARGS;
    }

    return OT_ERROR_NONE;
}

static otError ProcessIpaddrCmd(void *aContext, uint8_t aArgsLength, char *aArgs[])
{
    OT_UNUSED_VARIABLE(aContext);

    struct netif *n;

    if (aArgsLength == 0)
    {
        NETIF_FOREACH(n)
        {
            PrintIpaddr(n);
        }
    }
    else if (aArgsLength >= 1)
    {
        /* First arg must me interface */
        n = NetifFromArg(aArgs[0]);
        if (n == NULL)
        {
            return OT_ERROR_NOT_FOUND;
        }
        if (aArgsLength == 1)
        {
            PrintIpaddr(n);
        }
        else
        {
            if (strcmp("add", aArgs[1]) == 0)
            {
                return ProcessIpaddrAddCmd(n, aArgsLength - 2, &aArgs[2]);
            }
            else if (strcmp("del", aArgs[1]) == 0)
            {
                return ProcessIpaddrDelCmd(n, aArgsLength - 2, &aArgs[2]);
            }
            else
            {
                return OT_ERROR_INVALID_ARGS;
            }
        }
    }

    return OT_ERROR_NONE;
}

static void PrintLink(const struct netif *n)
{
    otCliOutputFormat("%c%c%u: ", n->name[0], n->name[1], n->num);
    if (n->hwaddr_len == 6)
    {
        otCliOutputFormat("%2x:%2x:%2x:%2x:%2x:%2x ", n->hwaddr[0], n->hwaddr[1], n->hwaddr[2], n->hwaddr[3],
                          n->hwaddr[4], n->hwaddr[5]);
    }

    otCliOutputFormat("%s", netif_is_up(n) ? "UP" : "DOWN");
    if (netif_is_up(n))
    {
        otCliOutputFormat(" LINK_%s", netif_is_link_up(n) ? "UP" : "DOWN");
    }

    otCliOutputFormat("\r\n");
}

static otError ProcessIplinkCmd(void *aContext, uint8_t aArgsLength, char *aArgs[])
{
    OT_UNUSED_VARIABLE(aContext);
    struct netif *n;

    if (aArgsLength == 0)
    {
        NETIF_FOREACH(n)
        {
            PrintLink(n);
        }
    }
    else if (aArgsLength == 1)
    {
        /* First arg must me interface */
        n = NetifFromArg(aArgs[0]);
        if (n == NULL)
        {
            return OT_ERROR_NOT_FOUND;
        }
        else
        {
            PrintLink(n);
        }
    }
    return OT_ERROR_NONE;
}

#if LWIP_STATS && LWIP_STATS_DISPLAY
static otError ProcessStatsCmd(void *aContext, uint8_t aArgsLength, char *aArgs[])
{
    OT_UNUSED_VARIABLE(aContext);
    char *arg;
    int   i;
    int   j;

    if (aArgsLength == 0U)
    {
        /* No argument provided, display all available statistics */
        stats_display();
        return OT_ERROR_NONE;
    }

#define PRINT_STATS_IF(stat_name, arg_name)                  \
    if ((stat_name##_STATS) && strcmp(arg, (arg_name)) == 0) \
    {                                                        \
        stat_name##_STATS_DISPLAY();                         \
        continue;                                            \
    }

#define PRINT_STATS_LIST_IF(stat_name, arg_name)             \
    if ((stat_name##_STATS) && strcmp(arg, (arg_name)) == 0) \
    {                                                        \
        for (j = 0; j < (stat_name##_MAX); j++)              \
        {                                                    \
            stat_name##_STATS_DISPLAY(j);                    \
        }                                                    \
        continue;                                            \
    }

    for (i = 0; i < aArgsLength; i++)
    {
        arg = aArgs[i];

        PRINT_STATS_IF(LINK, "link")
        PRINT_STATS_IF(ETHARP, "etharp")
        PRINT_STATS_IF(IPFRAG, "ipfrag")
        PRINT_STATS_IF(IP6_FRAG, "ip6_frag")
        PRINT_STATS_IF(IP, "ip")
        PRINT_STATS_IF(ND6, "nd6")
        PRINT_STATS_IF(IP6, "ip6")
        PRINT_STATS_IF(IGMP, "igmp")
        PRINT_STATS_IF(MLD6, "mld6")
        PRINT_STATS_IF(ICMP, "icmp")
        PRINT_STATS_IF(ICMP6, "icmp6")
        PRINT_STATS_IF(UDP, "udp")
        PRINT_STATS_IF(TCP, "tcp")
        PRINT_STATS_IF(MEM, "mem")
        PRINT_STATS_LIST_IF(MEMP, "memp")
        PRINT_STATS_IF(SYS, "sys")

        otCliOutputFormat("\r\ninvalid argument: %s\r\n", arg);
    }

    return OT_ERROR_NONE;
}
#endif /* LWIP_STATS && LWIP_STATS_DISPLAY */

static otError ProcessHelpCmd(void *aContext, uint8_t aArgsLength, char *aArgs[])
{
    OT_UNUSED_VARIABLE(aContext);
    OT_UNUSED_VARIABLE(aArgsLength);
    OT_UNUSED_VARIABLE(aArgs);

    otCliOutputFormat("lwip ipaddr [interface]\r\n");
    otCliOutputFormat("lwip ipaddr <interface> add <ipaddr>\r\n");
    otCliOutputFormat("lwip ipaddr <interface> del <ipaddr>\r\n");
    otCliOutputFormat("lwip iplink [interface]\r\n");
#if LWIP_STATS && LWIP_STATS_DISPLAY
    otCliOutputFormat("lwip stats");

#define PRINT_STATS_ARG(stat_name, arg_name)  \
    if (stat_name##_STATS)                    \
    {                                         \
        otCliOutputFormat(" [" arg_name "]"); \
    }

    PRINT_STATS_ARG(LINK, "link")
    PRINT_STATS_ARG(ETHARP, "etharp")
    PRINT_STATS_ARG(IPFRAG, "ipfrag")
    PRINT_STATS_ARG(IP6_FRAG, "ip6_frag")
    PRINT_STATS_ARG(IP, "ip")
    PRINT_STATS_ARG(ND6, "nd6")
    PRINT_STATS_ARG(IP6, "ip6")
    PRINT_STATS_ARG(IGMP, "igmp")
    PRINT_STATS_ARG(MLD6, "mld6")
    PRINT_STATS_ARG(ICMP, "icmp")
    PRINT_STATS_ARG(ICMP6, "icmp6")
    PRINT_STATS_ARG(UDP, "udp")
    PRINT_STATS_ARG(TCP, "tcp")
    PRINT_STATS_ARG(MEM, "mem")
    PRINT_STATS_ARG(MEMP, "memp")
    PRINT_STATS_ARG(SYS, "sys")
    /* MIB2_STATS omitted - there is no function to print MIB2 stats */

    otCliOutputFormat("\r\n");
#endif /* LWIP_STATS && LWIP_STATS_DISPLAY */

    return OT_ERROR_NONE;
}
