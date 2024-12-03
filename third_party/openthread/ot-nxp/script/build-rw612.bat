@echo off

if "%NXP_RW612_SDK_ROOT%" == "" (
    echo Variable NXP_RW612_SDK_ROOT is not set.
    echo Please set it to the location of the SDK and then re-build.
    goto :end
)

if not exist %NXP_RW612_SDK_ROOT% (
    echo '%NXP_RW612_SDK_ROOT%' is not a valid path.
    echo 'Please set NXP_RW612_SDK_ROOT to a valid path pointing to the SDK'
    goto :end
)


if exist %NXP_RW612_SDK_ROOT%\SW-Content-Register.txt (
    set SDK_RELEASE=ON
) else if exist %NXP_RW612_SDK_ROOT%\.gitmodules (
    set SDK_RELEASE=OFF
) else (
    echo Could not find a valid SDK at %NXP_RW612_SDK_ROOT%
    goto :end
)

echo NXP_RW612_SDK_ROOT set to %NXP_RW612_SDK_ROOT%
echo SDK_RELASE set to %SDK_RELEASE%

set OT_OPTIONS=-DCMAKE_TOOLCHAIN_FILE=cmake/toolchain/arm-none-eabi.cmake^
 -DCMAKE_EXPORT_COMPILE_COMMANDS=1^
 -DCMAKE_BUILD_TYPE=Debug^
 -DOT_NXP_EXPORT_TO_BIN=ON^
 -DOT_PLATFORM=external^
 -DOT_SLAAC=ON^
 -DOT_SERVICE=ON^
 -DOT_PING_SENDER=ON^
 -DOT_NXP_PLATFORM_FAMILY=rw^
 -DOT_NXP_PLATFORM=rw612^
 -DOT_NXP_BOARD=rdrw612bga^
 -DOT_EXTERNAL_HEAP=ON^
 -DOT_EXTERNAL_MBEDTLS=nxp-rw612-mbedtls^
 -DOT_NXP_MBEDTLS_PORT=els_pkc^
 -DOT_APP_CLI_FREERTOS=ON^
 -DOT_SETTINGS_RAM=ON^
 -DOT_STACK_ENABLE_LOG=OFF^
 -DOT_APP_NCP=OFF^
 -DOT_APP_RCP=OFF^
 -DOT_RCP=OFF^
 -DOT_APP_CLI=OFF^
 -DOT_APP_BR_FREERTOS=ON^
 -DOT_NXP_LWIP_ETH=OFF^
 -DOT_NXP_LWIP_WIFI=ON^
 -DOT_RCP_RESTORATION_MAX_COUNT=10^
 -DOT_COMPILE_WARNING_ASS_ERROR=ON^
 -DSDK_RELEASE=%SDK_RELEASE%
echo OT_OPTIONS %OT_OPTIONS%

set OT_SRCDIR=%CD%

if exist build_rw612 (
    rmdir build_rw612 \s
)

mkdir build_rw612
cd build_rw612

cmake -GNinja -DUSE_NBU=0 -DOT_COMPILE_WARNING_AS_ERROR=ON %OT_OPTIONS% %OT_SRCDIR%
ninja

cd %OT_SRCDIR%

:end
