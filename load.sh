#!/bin/sh

# Load a program to the Pico2

set -e

#PROGRAM=hello_led
#PROGRAM=hello_usb
#PROGRAM=hello_led_external
#PROGRAM=pomodoro_led
#PROGRAM=hello_uart
#PROGRAM=hello_lcd
#PROGRAM=hello_ultrasonic_ranging
#PROGRAM=hello_buzzer
#PROGRAM=hello_timer
#PROGRAM=hello_adc
#PROGRAM=hello_dma
#PROGRAM=hello_servo
#PROGRAM=hello_motor_L293D
#PROGRAM=hello_MPU6050
#PROGRAM=oscilloscope
#PROGRAM=servo_control
PROGRAM=buttons_pad

if [ "$PROGRAM" = "" ]; then
    echo "Error, PROGRAM not selected"
    exit 1
fi

if [ ! -f build/"$PROGRAM".uf2 ]; then
    echo "Error, build/$PROGRAM.uf2 not found"
    exit 1
fi

picotool load -x build/"$PROGRAM".uf2

echo Done!

