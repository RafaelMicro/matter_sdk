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

# Build script for GN EFT32 examples GitHub workflow.

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

USAGE="./scripts/examples/gn_rt582_example.sh <AppRootFolder> <outputFolder>[<Build options>]"

if [ "$#" == "0" ]; then
    echo "Build script for RT582 Matter apps
    Format:
    $USAGE

    <AppRootFolder>
        Root Location of the app e.g: examples/lighting-app/rt582/

    <outputFolder>
        Desired location for the output files

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
    "
elif [ "$#" -lt "2" ]; then
    echo "Invalid number of arguments
    Format:
    $USAGE"
else
    ROOT=$1
    OUTDIR=$2

    shift
    shift
    while [ $# -gt 0 ]; do
        case $1 in
        --sed)
            optArgs+="enable_sleepy_device=true chip_openthread_ftd=false "
            shift
            ;;
        *)

            optArgs+=$1" "
            shift
            ;;
        esac
    done

    BUILD_DIR=$OUTDIR/"RT582"
    echo BUILD_DIR="$BUILD_DIR"

    # thread build
    #
    if [ -z "$optArgs" ]; then
        gn gen --check --fail-on-unused-args --export-compile-commands --root="$ROOT" "$BUILD_DIR"
    else
        gn gen --check --fail-on-unused-args --export-compile-commands --root="$ROOT" --args="$optArgs" "$BUILD_DIR"
    fi
    # ninja -C "$BUILD_DIR"/
    ninja -v -C "$BUILD_DIR"/
    #print stats
    arm-none-eabi-size -A "$BUILD_DIR"/*.out
fi
