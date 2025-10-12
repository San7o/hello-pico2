cmake_minimum_required(VERSION 3.13...3.27)
# Set the board here
set(PICO_BOARD pico2)
# initialize pico-sdk from submodule
# note: this must happen before project()
include(pico-sdk/pico_sdk_init.cmake)

project(hello_pico)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

# Subprojects
include(hello_usb/hello_usb.cmake)
include(hello_led/hello_led.cmake)
include(hello_led_external/hello_led_external.cmake)
