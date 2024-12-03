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

#ifndef RADIO_CLI_H_
#define RADIO_CLI_H_

/* -------------------------------------------------------------------------- */
/*                                  Includes                                  */
/* -------------------------------------------------------------------------- */

#include <openthread/error.h>

/* -------------------------------------------------------------------------- */
/*                            NXP flags definition                            */
/* -------------------------------------------------------------------------- */

#ifndef OT_NXP_PLAT_TX_PWR_LIMIT_MAX
#define OT_NXP_PLAT_TX_PWR_LIMIT_MAX 44
#endif

#ifndef OT_NXP_PLAT_TX_PWR_HALF_DBM
#define OT_NXP_PLAT_TX_PWR_HALF_DBM 0
#endif

#ifndef OT_NXP_PLAT_MFG_FIRST_ANNEX_ID
#define OT_NXP_PLAT_MFG_FIRST_ANNEX_ID 55
#endif

#ifndef OT_NXP_PLAT_MFG_LAST_ANNEX_ID
#define OT_NXP_PLAT_MFG_LAST_ANNEX_ID 108
#endif

/* -------------------------------------------------------------------------- */
/*                              Public prototypes                             */
/* -------------------------------------------------------------------------- */

/*!
 * @brief Handler called from ot-cli when radio_nxp command is used
 *
 * @param[in] aContext pointer to a context, not used
 * @param[in] aArgsLength
 * @param[in] aArgs
 * @return otError
 */
otError ProcessRadio(void *aContext, uint8_t aArgsLength, char *aArgs[]);

#endif /* RADIO_CLI_H_ */
