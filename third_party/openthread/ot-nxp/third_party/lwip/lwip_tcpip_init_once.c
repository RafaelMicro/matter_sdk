/*
 * Copyright 2023 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 */

#include "lwip_tcpip_init_once.h"

#ifndef LWIP_TCPIP_INIT_CB_BACKLOG_SIZE
#define LWIP_TCPIP_INIT_CB_BACKLOG_SIZE 2
#endif

static u8_t init_done = 0;
static u8_t init_called = 0;
static s8_t backlog_ptr = 0;

static tcpip_init_done_fn cb_backlog[LWIP_TCPIP_INIT_CB_BACKLOG_SIZE];
static void* cb_arg_backlog[LWIP_TCPIP_INIT_CB_BACKLOG_SIZE];

static void init_done_cb(void* arg){
    LWIP_UNUSED_ARG(arg);
    init_done = 1;

    while(backlog_ptr > 0){
        backlog_ptr--;
        cb_backlog[backlog_ptr](cb_arg_backlog[backlog_ptr]);
    }
}

/**
 * Calls @ref tcpip_init if wasn't already called. Keeps backlog of initfunc callbacks and
 * calls them when init is done.
 *
 * @param initfunc a function to call when tcpip_thread is running and finished initializing
 * @param arg argument to pass to initfunc
 */
void tcpip_init_once(tcpip_init_done_fn initfunc, void *arg)
{
    if(!init_done){
        if(initfunc){
            LWIP_ASSERT("Not enough LWIP_TCPIP_INIT_CB_BACKLOG_SIZE", backlog_ptr < LWIP_TCPIP_INIT_CB_BACKLOG_SIZE);

            cb_backlog[backlog_ptr] = initfunc;
            cb_arg_backlog[backlog_ptr] = arg;
            backlog_ptr++;
        }
        if(!init_called){
            init_called = 1;
            tcpip_init(init_done_cb, NULL);
        }
    }else{
        if(initfunc){
            initfunc(arg);
        }
    }
}