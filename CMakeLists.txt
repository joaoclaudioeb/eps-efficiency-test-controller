cmake_minimum_required(VERSION 3.12)

set(PICO_SDK_PATH $ENV{PICO_SDK_PATH})
set(FREERTOS_KERNEL_PATH ${CMAKE_SOURCE_DIR}/os/FreeRTOS)

include(${PICO_SDK_PATH}/external/pico_sdk_import.cmake)
include(${FREERTOS_KERNEL_PATH}/portable/ThirdParty/GCC/RP2040/FreeRTOS_Kernel_import.cmake)

project(main_project)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS On)

pico_sdk_init()

add_library(freertos STATIC
  ${FREERTOS_KERNEL_PATH}
  ${FREERTOS_KERNEL_PATH}/portable/ThirdParty/GCC/RP2040/port.c
)

target_include_directories(freertos PUBLIC
  .
  ${FREERTOS_KERNEL_PATH}/include
  ${FREERTOS_KERNEL_PATH}/portable/ThirdParty/GCC/RP2040/include
)

target_link_libraries(freertos
  FreeRTOS-Kernel
  FreeRTOS-Kernel-Heap3
  pico_stdlib
)

add_executable(
  main
  main.cpp
  ${CMAKE_SOURCE_DIR}/drivers/uart/arch/rsp_pico.cpp
  ${CMAKE_SOURCE_DIR}/drivers/uart/uart_module.cpp
)

target_include_directories(main PUBLIC
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/drivers/
    ${CMAKE_SOURCE_DIR}/drivers/uart/
)

pico_enable_stdio_usb(main 1)

pico_add_extra_outputs(main)
target_link_libraries(main pico_stdlib freertos)
