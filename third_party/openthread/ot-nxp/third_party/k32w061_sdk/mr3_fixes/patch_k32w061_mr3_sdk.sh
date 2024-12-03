#!/bin/bash
#
# Copyright 2023 NXP
# All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
CURR_PATH="`dirname \"$0\"`" 

if [[ ! -d $NXP_K32W0_SDK_ROOT ]]; then
    echo "NXP_K32W0_SDK_ROOT is not set"
    exit 1
fi

cp $CURR_PATH/board_utility.c   "$NXP_K32W0_SDK_ROOT"/boards/k32w061dk6/wireless_examples/openthread/enablement
cp $CURR_PATH/board_utility.h   "$NXP_K32W0_SDK_ROOT"/boards/k32w061dk6/wireless_examples/openthread/enablement
cp $CURR_PATH/startup_K32W061.c "$NXP_K32W0_SDK_ROOT"/devices/K32W061/gcc

echo "K32W061 SDK Master Release 3 was patched!"
exit 0
