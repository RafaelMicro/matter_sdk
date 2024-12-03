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

#ifndef TOKEN_BUCKET_H_
#define TOKEN_BUCKET_H_

/* -------------------------------------------------------------------------- */
/*                                  Includes                                  */
/* -------------------------------------------------------------------------- */

#include <stdbool.h>
#include <stdint.h>

#include <FreeRTOS.h>
#include <semphr.h>
#include <timers.h>

/* -------------------------------------------------------------------------- */
/*                                 Definitions                                */
/* -------------------------------------------------------------------------- */

/**
 * A token bucket structure used for rate limiting.
 */
typedef struct otTokenBucket
{
    SemaphoreHandle_t mutex;     ///< Guards access to the tokens.
    TimerHandle_t     timer;     ///< Timer which refills the bucket with tokens.
    uint32_t          rate;      ///< Rate per second in thousandths. Also sets the maximum limit for tokens.
    uint32_t          tokens;    ///< Actual number of tokens in the bucket in thousandths of tokens.
    uint32_t          increment; ///< Number of thousands to increment the token number with when timer fires.
} otTokenBucket;

/* -------------------------------------------------------------------------- */
/*                             Public prototypes                              */
/* -------------------------------------------------------------------------- */

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @brief This function initializes the token bucket
 *
 * @param[in] aBucket pointer to the token bucket structure
 * @param[in] rate maximum rate (tokens per second)
 */
void otTokenBucketInit(otTokenBucket *aBucket, uint32_t rate);

/*!
 * @brief This function tries to take requested number of tokens from the bucket
 *
 * @param[in] aBucket pointer to the token bucket structure
 * @param[in] tokens number of tokens requested to be taken from the bucket
 * @return the requested number of tokens or 0 if not enough tokens in the bucket
 */
uint32_t otTokenBucketTake(otTokenBucket *aBucket, uint32_t tokens);

/*!
 * @brief This function checks if the requested number of tokens is in the bucket.
 *
 * @param[in] aBucket pointer to the token bucket structure
 * @param[in] tokens number of tokens
 * @return true if the requested number of tokens is in the bucket, false otherwise
 */
bool otTokenBucketCanTake(otTokenBucket *aBucket, uint32_t tokens);

#ifdef __cplusplus
}
#endif

#endif /* TOKEN_BUCKET_H_ */
