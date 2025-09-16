
if((${CONFIG_CHIP} STREQUAL "RT581")
   OR (${CONFIG_CHIP} STREQUAL "RT582")
   OR (${CONFIG_CHIP} STREQUAL "RT583"))
sdk_generate_library(rt582_freertos)
sdk_add_include_directories(
    portable/GCC/ARM_CM3/
    Include
)
sdk_library_add_sources(
    croutine.c
    event_groups.c
    list.c
    queue.c
    stream_buffer.c
    tasks.c
    timers.c
    portable/GCC/ARM_CM3/port.c
    portable/MemMang/heap_5.c
)

elseif(
  (${CONFIG_CHIP} STREQUAL "RT584S")
  OR (${CONFIG_CHIP} STREQUAL "RT584H")
  OR (${CONFIG_CHIP} STREQUAL "RT584L"))

    sdk_generate_library(rt584_freertos)

    sdk_library_add_sources(
        croutine.c
        event_groups.c
        list.c
        queue.c
        stream_buffer.c
        tasks.c
        timers.c
        portable/MemMang/heap_5.c
    )

    if((${CONFIG_TRUST_ZONE} STREQUAL "TRUST_ZONE_SECURE"))
        sdk_library_add_sources(
        #portable/GCC/ARM_CM33/secure/secure_context_port.c
        #portable/GCC/ARM_CM33/secure/secure_context.c
        #portable/GCC/ARM_CM33/secure/secure_heap.c
        #portable/GCC/ARM_CM33/secure/secure_init.c
        #portable/GCC/ARM_CM33/non_secure/port.c
        #portable/GCC/ARM_CM33/non_secure/portasm.c
        portable/GCC/ARM_CM33_NTZ/non_secure/port.c
        portable/GCC/ARM_CM33_NTZ/non_secure/portasm.c
    )
    elseif((${CONFIG_TRUST_ZONE} STREQUAL "TRUST_ZONE_NONSECURE"))
        sdk_library_add_sources(
        portable/GCC/ARM_CM33_NTZ/non_secure/port.c
        portable/GCC/ARM_CM33_NTZ/non_secure/portasm.c
    )
    endif()

    sdk_add_include_directories(
        Include
    )

    if((${CONFIG_TRUST_ZONE} STREQUAL "TRUST_ZONE_SECURE"))
        sdk_add_include_directories(
        #portable/GCC/ARM_CM33/secure
        #portable/GCC/ARM_CM33/non_secure
        portable/GCC/ARM_CM33_NTZ/non_secure
    )
    elseif((${CONFIG_TRUST_ZONE} STREQUAL "TRUST_ZONE_NONSECURE"))
        sdk_add_include_directories(
        portable/GCC/ARM_CM33/non_secure
    )
    endif()
    
endif()
