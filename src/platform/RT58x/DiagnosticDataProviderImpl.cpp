/*
 *
 *    Copyright (c) 2021 Project CHIP Authors
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */

/**
 *    @file
 *          Provides an implementation of the DiagnosticDataProvider object
 *          for RT58x platform.
 */

#include <platform/internal/CHIPDeviceLayerInternal.h>

#include <platform/DiagnosticDataProvider.h>
#include <platform/RT58x/DiagnosticDataProviderImpl.h>
#if CHIP_DEVICE_CONFIG_ENABLE_THREAD
#include <platform/OpenThread/GenericThreadStackManagerImpl_OpenThread.h>
#endif
#include "AppConfig.h"
#include "FreeRTOS.h"
// #include "heap_4_silabs.h"
#include <lib/support/CHIPMemString.h>

using namespace ::chip::app::Clusters::GeneralDiagnostics;

namespace chip {
namespace DeviceLayer {

DiagnosticDataProviderImpl & DiagnosticDataProviderImpl::GetDefaultInstance()
{
    static DiagnosticDataProviderImpl sInstance;
    return sInstance;
}

// Software Diagnostics Getters
/*
 * The following Heap stats are compiled values done by the FreeRTOS Heap4 implementation.
 * See /examples/platform/efr32/heap_4_silabs.c
 * It keeps track of the number of calls to allocate and free memory as well as the
 * number of free bytes remaining, but says nothing about fragmentation.
 */
CHIP_ERROR DiagnosticDataProviderImpl::GetCurrentHeapFree(uint64_t & currentHeapFree)
{
    size_t freeHeapSize = xPortGetFreeHeapSize();
    currentHeapFree     = static_cast<uint64_t>(freeHeapSize);
    return CHIP_NO_ERROR;
}

CHIP_ERROR DiagnosticDataProviderImpl::GetCurrentHeapUsed(uint64_t & currentHeapUsed)
{
    // Calculate the Heap used based on Total heap - Free heap
    int64_t heapUsed = (configTOTAL_HEAP_SIZE - xPortGetFreeHeapSize());

    // Something went wrong, this should not happen
    VerifyOrReturnError(heapUsed >= 0, CHIP_ERROR_INVALID_INTEGER_VALUE);
    currentHeapUsed = static_cast<uint64_t>(heapUsed);
    return CHIP_NO_ERROR;
}


DiagnosticDataProvider & GetDiagnosticDataProviderImpl()
{
    return DiagnosticDataProviderImpl::GetDefaultInstance();
}

} // namespace DeviceLayer
} // namespace chip
