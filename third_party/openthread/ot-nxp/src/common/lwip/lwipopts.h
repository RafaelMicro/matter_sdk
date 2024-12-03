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

#ifndef __LWIPOPTS_H__
#define __LWIPOPTS_H__

#define IP_REASSEMBLY 0
#define ETH_DO_RX_IN_SEPARATE_TASK 0

/**
 * NO_SYS==0: Use RTOS
 */
#define NO_SYS 0

#define ENET_RXBD_NUM (17)

#define LWIP_IPV6_FORWARD 1

#if OPENTHREAD_CONFIG_NAT64_TRANSLATOR_ENABLE
/* Enable IPv4 forwarding for NAT64 */
#define IP_FORWARD 1
#endif /* OPENTHREAD_CONFIG_NAT64_TRANSLATOR_ENABLE */

#define LWIP_IPV6_NUM_ADDRESSES 8

#define LWIP_IPV6_SEND_ROUTER_SOLICIT 1

#define LWIP_IPV6_DHCP6 1
#define LWIP_IPV6_DHCP6_STATEFUL 1
#define LWIP_IPV6_DHCP6_PD 1

#define CONFIG_NETWORK_HIGH_PERF 1
//#define LWIP_ALLOW_MEM_FREE_FROM_OTHER_CONTEXT 1

#define MAX_SOCKETS_TCP 8
#define MAX_LISTENING_SOCKETS_TCP 4
#define MAX_SOCKETS_UDP 10
#define TCP_SND_BUF_COUNT 2
#define TCPIP_STACK_TX_HEAP_SIZE 0
#define LWIP_COMPAT_SOCKETS 2

/**
 * Loopback demo related options.
 */
#define LWIP_NETIF_LOOPBACK 1
#define LWIP_HAVE_LOOPIF 1
#define LWIP_NETIF_LOOPBACK_MULTITHREADING 1
#define LWIP_LOOPBACK_MAX_PBUFS 8

#define TCPIP_THREAD_NAME "tcp/ip"
#define TCPIP_THREAD_STACKSIZE 1024
#define TCPIP_THREAD_PRIO 2
#ifdef CONFIG_NETWORK_HIGH_PERF
#define TCPIP_MBOX_SIZE 64
#else
#define TCPIP_MBOX_SIZE 32
#endif

/**
 * DEFAULT_RAW_RECVMBOX_SIZE: The mailbox size for the incoming packets on a
 * NETCONN_RAW. The queue size value itself is platform-dependent, but is passed
 * to sys_mbox_new() when the recvmbox is created.
 */
#ifdef CONFIG_NETWORK_HIGH_PERF
#define DEFAULT_RAW_RECVMBOX_SIZE 32
#else
#define DEFAULT_RAW_RECVMBOX_SIZE 12
#endif

/**
 * DEFAULT_UDP_RECVMBOX_SIZE: The mailbox size for the incoming packets on a
 * NETCONN_UDP. The queue size value itself is platform-dependent, but is passed
 * to sys_mbox_new() when the recvmbox is created.
 */
#ifdef CONFIG_NETWORK_HIGH_PERF
#define DEFAULT_UDP_RECVMBOX_SIZE 32
#else
#define DEFAULT_UDP_RECVMBOX_SIZE 12
#endif

/**
 * DEFAULT_TCP_RECVMBOX_SIZE: The mailbox size for the incoming packets on a
 * NETCONN_TCP. The queue size value itself is platform-dependent, but is passed
 * to sys_mbox_new() when the recvmbox is created.
 */
#ifdef CONFIG_NETWORK_HIGH_PERF
#define DEFAULT_TCP_RECVMBOX_SIZE 32
#else
#define DEFAULT_TCP_RECVMBOX_SIZE 12
#endif

/**
 * DEFAULT_ACCEPTMBOX_SIZE: The mailbox size for the incoming connections.
 * The queue size value itself is platform-dependent, but is passed to
 * sys_mbox_new() when the acceptmbox is created.
 */
#define DEFAULT_ACCEPTMBOX_SIZE 12

#define DEFAULT_THREAD_STACKSIZE 512
#define DEFAULT_THREAD_PRIO 1

#define SYS_LIGHTWEIGHT_PROT 1

/*
   ------------------------------------
   ---------- Memory options ----------
   ------------------------------------
*/

#define MEM_ALIGNMENT 4

/* Value of TCP_SND_BUF_COUNT denotes the number of buffers and is set by
 * CONFIG option available in the SDK
 */
#ifdef CONFIG_NETWORK_HIGH_PERF
#define TCP_SND_BUF (12 * TCP_MSS)
#else
#define TCP_SND_BUF (TCP_SND_BUF_COUNT * TCP_MSS)
#endif

/* Buffer size needed for TCP: Max. number of TCP sockets * Size of pbuf *
 * Max. number of TCP sender buffers per socket
 *
 * Listening sockets for TCP servers do not require the same amount buffer
 * space. Hence do not consider these sockets for memory computation
 */
#define TCP_MEM_SIZE (MAX_SOCKETS_TCP * PBUF_POOL_BUFSIZE * (TCP_SND_BUF / TCP_MSS))

/* Buffer size needed for UDP: Max. number of UDP sockets * Size of pbuf
 */
#define UDP_MEM_SIZE (MAX_SOCKETS_UDP * PBUF_POOL_BUFSIZE)

/**
 * MEM_SIZE: the size of the heap memory. If the application will send
 * a lot of data that needs to be copied, this should be set high.
 */
#if (TCPIP_STACK_TX_HEAP_SIZE == 0)
#define MEM_SIZE (TCP_MEM_SIZE + UDP_MEM_SIZE)
#else
#define MEM_SIZE (TCPIP_STACK_TX_HEAP_SIZE * 1024)
#endif

#ifdef CONFIG_NETWORK_HIGH_PERF
#undef MEM_SIZE
#define MEM_SIZE (40 * 1024)
#endif

/*
   ------------------------------------------------
   ---------- Internal Memory Pool Sizes ----------
   ------------------------------------------------
*/
#define MEMP_USE_CUSTOM_POOLS 0

/**
 * MEMP_NUM_PBUF: the number of memp struct pbufs (used for PBUF_ROM and PBUF_REF).
 * If the application sends a lot of data out of ROM (or other static memory),
 * this should be set high.
 */
#ifdef CONFIG_NETWORK_HIGH_PERF
#define MEMP_NUM_PBUF 20
#else
#define MEMP_NUM_PBUF 10
#endif

/**
 * MEMP_NUM_RAW_PCB: Number of raw connection PCBs
 * (requires the LWIP_RAW option)
 */
#ifndef MEMP_NUM_RAW_PCB
#if OPENTHREAD_CONFIG_NAT64_TRANSLATOR_ENABLE
/* Increase default as three PCBs are used for NAT64 */
#define MEMP_NUM_RAW_PCB 7
#endif /* OPENTHREAD_CONFIG_NAT64_TRANSLATOR_ENABLE */
#endif /* MEMP_NUM_RAW_PCB */

/**
 * MEMP_NUM_TCP_PCB: the number of simulatenously active TCP connections.
 * (requires the LWIP_TCP option)
 */
#define MEMP_NUM_TCP_PCB MAX_SOCKETS_TCP

#define MEMP_NUM_TCP_PCB_LISTEN MAX_LISTENING_SOCKETS_TCP

/**
 * MEMP_NUM_TCP_SEG: the number of simultaneously queued TCP segments.
 * (requires the LWIP_TCP option)
 */
#ifdef CONFIG_NETWORK_HIGH_PERF
#define MEMP_NUM_TCP_SEG 48
#else
#define MEMP_NUM_TCP_SEG 12
#endif

/**
 * MEMP_NUM_TCPIP_MSG_INPKT: the number of struct tcpip_msg, which are used
 * for incoming packets.
 * (only needed if you use tcpip.c)
 */
#ifdef CONFIG_NETWORK_HIGH_PERF
#define MEMP_NUM_TCPIP_MSG_INPKT 32
#else
#define MEMP_NUM_TCPIP_MSG_INPKT 16
#endif

/** MEMP_NUM_TCPIP_MSG_*: the number of struct tcpip_msg, which is used
   for sequential API communication and incoming packets. Used in
   src/api/tcpip.c. */
#ifdef CONFIG_NETWORK_HIGH_PERF
#define MEMP_NUM_TCPIP_MSG_API 16
#else
#define MEMP_NUM_TCPIP_MSG_API 8
#endif

/**
 * MEMP_NUM_SYS_TIMEOUT: the number of simulateously active timeouts.
 * (requires NO_SYS==0)
 */
#define MEMP_NUM_SYS_TIMEOUT 20

/**
 * MEMP_NUM_NETBUF: the number of struct netbufs.
 * (only needed if you use the sequential API, like api_lib.c)
 */
#ifdef CONFIG_NETWORK_HIGH_PERF
#define MEMP_NUM_NETBUF 32
#else
#define MEMP_NUM_NETBUF 16
#endif

/**
 * MEMP_NUM_NETCONN: the number of struct netconns.
 * (only needed if you use the sequential API, like api_lib.c)
 *
 * This number corresponds to the maximum number of active sockets at any
 * given point in time. This number must be sum of max. TCP sockets, max. TCP
 * sockets used for listening, and max. number of UDP sockets
 */
#define MEMP_NUM_NETCONN (MAX_SOCKETS_TCP + MAX_LISTENING_SOCKETS_TCP + MAX_SOCKETS_UDP)

/**
 * PBUF_POOL_SIZE: the number of buffers in the pbuf pool.
 */
#define PBUF_POOL_SIZE 40

/*
   ----------------------------------
   ---------- Pbuf options ----------
   ----------------------------------
*/

/**
 * PBUF_POOL_BUFSIZE: the size of each pbuf in the pbuf pool. The default is
 * designed to accomodate single full size TCP frame in one pbuf, including
 * TCP_MSS, IP header, and link header.
 */
#define PBUF_POOL_BUFSIZE 1580

/**
 * MEMP_NUM_FRAG_PBUF: the number of IP fragments simultaneously sent
 * (fragments, not whole packets!).
 * This is only used with LWIP_NETIF_TX_SINGLE_PBUF==0 and only has to be > 1
 * with DMA-enabled MACs where the packet is not yet sent when netif->output
 * returns.
 */
#define MEMP_NUM_FRAG_PBUF 15

/**
 * MEMP_NUM_REASSDATA: the number of IP packets simultaneously queued for
 * reassembly (whole packets, not fragments!)
 */
//#define MEMP_NUM_REASSDATA 8 FIXME
#define MEMP_NUM_REASSDATA 8

/**
 * IP_REASS_MAXAGE: Maximum time (in multiples of IP_TMR_INTERVAL - so seconds, normally)
 * a fragmented IP packet waits for all fragments to arrive. If not all fragments arrived
 * in this time, the whole packet is discarded.
 */
#define IP_REASS_MAXAGE 3

/**
 * IP_REASS_MAX_PBUFS: Total maximum amount of pbufs waiting to be reassembled.
 * Since the received pbufs are enqueued, be sure to configure
 * PBUF_POOL_SIZE > IP_REASS_MAX_PBUFS so that the stack is still able to receive
 * packets even if the maximum amount of fragments is enqueued for reassembly!
 * When IPv4 *and* IPv6 are enabled, this even changes to
 * (PBUF_POOL_SIZE > 2 * IP_REASS_MAX_PBUFS)!
 */
//#define IP_REASS_MAX_PBUFS 16 FIXME
#define IP_REASS_MAX_PBUFS 16

/**
 * TCP_MSS: TCP Maximum segment size. (default is 536, a conservative default,
 * you might want to increase this.)
 * For the receive side, this MSS is advertised to the remote side
 * when opening a connection. For the transmit size, this MSS sets
 * an upper limit on the MSS advertised by the remote host.
 */
#define TCP_MSS 1460

/*
   ---------------------------------
   ---------- RAW options ----------
   ---------------------------------
*/
/**
 * LWIP_RAW==1: Enable application layer to hook into the IP layer itself.
 */
#define LWIP_RAW 1

/*
   ---------------------------------------
   ---------- IPv6 options ---------------
   ---------------------------------------
*/
/**
 * LWIP_IPV6==1: Enable IPv6
 */
#define LWIP_IPV6 1
#define LWIP_NETIF_IPV6_STATUS_CALLBACK LWIP_IPV6

#define LWIP_DNS_SECURE 0

/*
   ------------------------------------
   ---------- Socket options ----------
   ------------------------------------
*/
/**
 * LWIP_SOCKET==1: Enable Socket API (require to use sockets.c)
 */
#define LWIP_SOCKET 1
#define LWIP_NETIF_API 1

/**
 * LWIP_RECV_CB==1: Enable callback when a socket receives data.
 */
#define LWIP_RECV_CB 1
/**
 * SO_REUSE==1: Enable SO_REUSEADDR option.
 */
#define SO_REUSE 1
#define SO_REUSE_RXTOALL 1

/**
 * TCP_WND: The size of a TCP window.  This must be at least
 * (2 * TCP_MSS) for things to work well
 **/
#ifdef CONFIG_NETWORK_HIGH_PERF
#define TCP_WND (32 * TCP_MSS)
#else
#define TCP_WND (10 * TCP_MSS)
#endif

/**
 * Enable TCP_KEEPALIVE
 */
#define LWIP_TCP_KEEPALIVE 1

/*
   ----------------------------------------
   ---------- Statistics options ----------
   ----------------------------------------
*/
/**
 * LWIP_STATS==1: Enable statistics collection in lwip_stats.
 */
#define LWIP_STATS 1

/**
 * LWIP_STATS_DISPLAY==1: Compile in the statistics output functions.
 */
#define LWIP_STATS_DISPLAY 1

/*
   ----------------------------------------
   - Platform specific diagnostic output. -
   ----------------------------------------
*/
#ifndef LWIP_PLATFORM_DIAG
#include <openthread/cli.h>

#define LWIP_PLATFORM_DIAG(x) \
    do                        \
    {                         \
        otCliOutputFormat x;  \
    } while (0)
#endif /* LWIP_PLATFORM_DIAG */

/*
   ----------------------------------
   ---------- DHCP options ----------
   ----------------------------------
*/
/**
 * LWIP_DHCP==1: Enable DHCP module.
 */
#define LWIP_DHCP 1
#define LWIP_NETIF_EXT_STATUS_CALLBACK 1

/**
 * DNS related options, revisit later to fine tune.
 */
#define LWIP_DNS 1
#define DNS_TABLE_SIZE 2       // number of table entries, default 4
#define DNS_MAX_NAME_LENGTH 64 // max. name length, default 256
#define DNS_MAX_SERVERS 2      // number of DNS servers, default 2
#define DNS_DOES_NAME_CHECK 1  // compare received name with given,def 0
#define DNS_MSG_SIZE 512
#define MDNS_MSG_SIZE 512

#define MDNS_TABLE_SIZE 1  // number of mDNS table entries
#define MDNS_MAX_SERVERS 1 // number of mDNS multicast addresses
/* TODO: Number of active UDP PCBs is equal to number of active UDP sockets plus
 * two. Need to find the users of these 2 PCBs
 */
#define MEMP_NUM_UDP_PCB (MAX_SOCKETS_UDP + 2)
/* NOTE: some times the socket() call for SOCK_DGRAM might fail if you dont
 * have enough MEMP_NUM_UDP_PCB */

/*
   ----------------------------------
   ---------- IGMP options ----------
   ----------------------------------
*/
/**
 * LWIP_IGMP==1: Turn on IGMP module.
 */
#define LWIP_IGMP 1

/**
 * LWIP_SO_SNDTIMEO==1: Enable send timeout for sockets/netconns and
 * SO_SNDTIMEO processing.
 */
#define LWIP_SO_SNDTIMEO 1

/**
 * LWIP_SO_RCVTIMEO==1: Enable receive timeout for sockets/netconns and
 * SO_RCVTIMEO processing.
 */
#define LWIP_SO_RCVTIMEO 1
#define LWIP_SO_SNDTIMEO 1
/**
 * TCP_LISTEN_BACKLOG==1: Handle backlog connections.
 */
#define TCP_LISTEN_BACKLOG 1

/* wmsdk; This is not needed now as error codes are taken from standard libc */
#define LWIP_PROVIDE_ERRNO 1
#define ERRNO 1

//#define LWIP_SNMP 1

/*
   ------------------------------------------------
   ---------- Network Interfaces options ----------
   ------------------------------------------------
*/
/**
 * LWIP_NETIF_HOSTNAME==1: use DHCP_OPTION_HOSTNAME with netif's hostname
 * field.
 */
#define LWIP_NETIF_HOSTNAME 1

/**
 * TCP_RESOURCE_FAIL_RETRY_LIMIT: limit for retrying sending of tcp segment
 * on resource failure error returned by driver.
 */
#define TCP_RESOURCE_FAIL_RETRY_LIMIT 50

#define LWIP_COMPAT_MUTEX_ALLOWED 1

/**
 * LWIP_CHECKSUM_ON_COPY==1: Calculate checksum when copying data from
 * application buffers to pbufs.
 */
#define LWIP_CHECKSUM_ON_COPY 1

/**
 * LWIP_CHKSUM_ALGORITHM==3: Use the optimised checksum algorithm.
 */
#define LWIP_CHKSUM_ALGORITHM 3

/* Hooks are relevant only for a border router */
#if OT_APP_BR_LWIP_HOOKS_EN

/* Header file with lwIP hooks */
#define LWIP_HOOK_FILENAME "lwip_hooks.h"

/* Hook for multicast forwarding and other filtering */
#define LWIP_HOOK_IP6_CANFORWARD lwipCanForwardHook

/* Hook for filtering of input traffic */
#define LWIP_HOOK_IP6_INPUT lwipInputHook

#endif /* OT_APP_BR_LWIP_HOOKS_EN */

#if (LWIP_DNS || LWIP_IGMP || LWIP_IPV6) && !defined(LWIP_RAND)
/* When using IGMP or IPv6, LWIP_RAND() needs to be defined to a random-function returning an u32_t random value*/
#include "lwip/arch.h"
u32_t lwip_rand(void);
#define LWIP_RAND() lwip_rand()
#endif

#define LWIP_NUM_NETIF_CLIENT_DATA 10
#define LWIP_TCPIP_CORE_LOCKING 1
#define LWIP_NETCONN_SEM_PER_THREAD 0
#define LWIP_NETCONN_FULLDUPLEX 0
// Note: According to Thread Conformance v1.2.0, a Thread Border Router MUST be able to hold a Multicast Listeners Table
//  in memory with at least seventy five (75) entries.
#define MEMP_NUM_MLD6_GROUP 10 + 75
#endif /* __LWIPOPTS_H__ */
