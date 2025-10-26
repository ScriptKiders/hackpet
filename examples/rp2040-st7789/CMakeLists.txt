cmake_minimum_required(VERSION 3.22)

set(PICO_SDK_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../libs/pico-sdk")

if(NOT EXISTS "${PICO_SDK_PATH}/pico_sdk_init.cmake")
    message(FATAL_ERROR "  Pico SDK NOT FOUND!")
endif()

message(STATUS "Using Pico SDK from: ${PICO_SDK_PATH}")

include(${PICO_SDK_PATH}/pico_sdk_init.cmake)

project(st7789_example C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_executable(st7789_example
    main.cpp
    st7789.cpp
)

target_link_libraries(st7789_example
    pico_stdlib
    hardware_spi
    hardware_gpio
)

pico_add_extra_outputs(st7789_example)
pico_enable_stdio_usb(st7789_example 1)
pico_enable_stdio_uart(st7789_example 0)