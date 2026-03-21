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
include(hello_uart/hello_uart.cmake)
include(hello_led/hello_led.cmake)
#include(hello_lcd/hello_lcd.cmake)
include(hello_led_external/hello_led_external.cmake)
include(hello_ultrasonic_ranging/hello_ultrasonic_ranging.cmake)
include(hello_buzzer/hello_buzzer.cmake)
include(hello_adc/hello_adc.cmake)
include(hello_timer/hello_timer.cmake)
include(hello_dma/hello_dma.cmake)
include(hello_servo/hello_servo.cmake)
include(hello_motor_L293D/hello_motor_L293D.cmake)
include(oscilloscope/oscilloscope.cmake)
include(servo_control/servo_control.cmake)
include(pomodoro_led/pomodoro_led.cmake)
