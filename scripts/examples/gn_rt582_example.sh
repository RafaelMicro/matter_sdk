#!/usr/bin/env bash

#
#    Copyright (c) 2020 Project CHIP Authors
#
#    Licensed under the Apache License, Version 2.0 (the "License");
#    you may not use this file except in compliance with the License.
#    You may obtain a copy of the License at
#
#        http://www.apache.org/licenses/LICENSE-2.0
#
#    Unless required by applicable law or agreed to in writing, software
#    distributed under the License is distributed on an "AS IS" BASIS,
#    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#    See the License for the specific language governing permissions and
#    limitations under the License.
#

# Build script for GN RT58x examples GitHub workflow.

set -e

if [[ -z "${MATTER_ROOT}" ]]; then
    echo "Using default path for Matter root"
    CHIP_ROOT="$(dirname "$0")/../.."
else
    echo "Using ENV path for Matter root"
    CHIP_ROOT="$MATTER_ROOT"
fi

source "$CHIP_ROOT/scripts/activate.sh"

set -x
env
USE_WIFI=false

USAGE="./scripts/examples/gn_rt582_example.sh <AppRootFolder> <outputFolder> <rafael_board_name> [<Build options>]"

if [ "$#" == "0" ]; then
    echo "Build script for RT582 Matter apps
    Format:
    $USAGE

    <AppRootFolder>
        Root Location of the app e.g: examples/lighting-app/rt582/

    <outputFolder>
        Desired location for the output files

    <rafael_board_name>
        Identifier of the board for which this app is built
        Currently Supported :
            RT582
            RT583

    <Build options> - optional noteworthy build options for RT582
        chip_build_libshell
            Enable libshell support. (Default false)
        chip_logging
            Current value (Default true)
        chip_openthread_ftd
            Use openthread Full Thread Device, else, use Minimal Thread Device. (Default true)
        enable_openthread_cli
            Enables openthread cli without matter shell. (Default true)
        kvs_max_entries
            Set the maxium Kvs entries that can be store in NVM (Default 75)
            Thresholds: 30 <= kvs_max_entries <= 255
        setupDiscriminator
            Discriminatoor value used for BLE connexion. (Default 3840)
        setupPinCode
            PIN code for PASE session establishment. (Default 20202021)
        enable_sleepy_device
            Enable Sleepy end device. (Default false)
            Must also set chip_openthread_ftd=false
        'import("//with_pw_rpc.gni")'
            Use to build the example with pigweed RPC
        OTA_periodic_query_timeout
            Periodic query timeout variable for OTA in seconds
        Presets
        --sed
            enable sleepy end device, set thread mtd
            For minimum consumption, disable openthread cli and qr code
        --additional_data_advertising
            enable Addition data advertissing and rotating device ID
        --use_ot_lib
            use the silabs openthread library
        --rafael-ota
            matter ota for rafael rt583
        --rafael-certs
            rafael dac/pai/cd certificates
    "
elif [ "$#" -lt "2" ]; then
    echo "Invalid number of arguments
    Format:
    $USAGE"
else
    ROOT=$1
    OUTDIR=$2

    if [ "$#" -gt "2" ]; then
        RAFAEL_BOARD=$3
        shift
    fi

    shift
    shift
    while [ $# -gt 0 ]; do
        case $1 in
        --sed=yes)
            optArgs+="enable_sleepy_device=true chip_openthread_ftd=false chip_build_libshell=false "
            shift
            ;;

        --ota=yes)
            # echo $1
            if [ "$RAFAEL_BOARD" == "RT582" ]; then
                echo "RT582 is not implemented OTA yet"
                exit 1
            fi
            optArgs+="chip_enable_ota_requestor=true "
            shift
            ;;

        --certs=yes)
            if [ "$RAFAEL_BOARD" == "RT582" ]; then
                echo "RT582 is not implemented DAC/PAI yet./"
                exit 1
            fi
            optArgs+="chip_build_platform_attestation_credentials_provider=true "
            # optArgs+="chip_build_platform_attestation_credentials_provider=true chip_use_transitional_commissionable_data_provider=false "
            shift
            ;;
        *)
            shift
            ;;
        esac
    done

    if [ "$RAFAEL_BOARD" != "RT582" ] && [ "$RAFAEL_BOARD" != "RT583" ]; then
        echo "RAFAEL_BOARD is not defined"
        exit 1
    fi

    BUILD_DIR=$OUTDIR/$RAFAEL_BOARD
    echo BUILD_DIR="$BUILD_DIR"

    # thread build
    #
    if [ -z "$optArgs" ]; then
        gn gen --check --fail-on-unused-args --export-compile-commands --root="$ROOT" --args="rafael_board=\"$RAFAEL_BOARD\"" "$BUILD_DIR"
    else
        gn gen --check --fail-on-unused-args --export-compile-commands --root="$ROOT" --args="rafael_board=\"$RAFAEL_BOARD\" $optArgs" "$BUILD_DIR"
    fi
    ninja -C "$BUILD_DIR"/
    # ninja -v -C "$BUILD_DIR"/
    # print stats
    arm-none-eabi-size -A "$BUILD_DIR"/*.out
fi
