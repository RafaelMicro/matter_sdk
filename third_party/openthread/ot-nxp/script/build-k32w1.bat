@echo off
if "%NXP_K32W1_SDK_ROOT%" == "" (
	echo Variable NXP_K32W1_SDK_ROOT is not set.

	echo Please set it to the location of the SDK and then re-build.
	goto :end
)

if not exist %NXP_K32W1_SDK_ROOT% (
	echo '%NXP_K32W1_SDK_ROOT%' is not a valid path.
	echo 'Please set NXP_K32W1_SDK_ROOT to a valid path pointing to the SDK'
	goto :end
)


if exist %NXP_K32W1_SDK_ROOT%\SW-Content-Register.txt (
	set SDK_RELEASE=ON
) else if exist %NXP_K32W1_SDK_ROOT%\.gitmodules (
	set SDK_RELEASE=OFF
) else (
	echo Could not find a valid SDK at %NXP_K32W1_SDK_ROOT%
	goto :end
)

echo NXP_K32W1_SDK_ROOT set to %NXP_K32W1_SDK_ROOT%
echo SDK_RELASE set to %SDK_RELEASE%

set OT_OPTIONS=-DCMAKE_TOOLCHAIN_FILE=cmake/toolchain/arm-none-eabi.cmake^
 -DCMAKE_EXPORT_COMPILE_COMMANDS=1^
 -DCMAKE_BUILD_TYPE=Debug^
 -DOT_NXP_EXPORT_TO_SREC=ON^
 -DOT_PLATFORM=external^
 -DOT_SLAAC=ON^
 -DSDK_RELEASE=%SDK_RELEASE%^
 -DOT_NXP_PLATFORM=k32w1^
 -DOT_NXP_PLATFORM_FAMILY=k32w1^
 -DOT_APP_LOWPOWER=ON

echo OT_OPTIONS %OT_OPTIONS%

set OT_SRCDIR=%CD%

if exist build_k32w1 (
	rmdir build_k32w1
)

mkdir build_k32w1
cd build_k32w1

cmake -GNinja -DOT_BUILD_K32W1=ON -DUSE_NBU=1 -DOT_COMPILE_WARNING_AS_ERROR=ON %OT_OPTIONS% %OT_SRCDIR%
ninja

cd %OT_SRCDIR%

:end
