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
PROGRAM=hello_adc
DEVICE=/dev/sdb1
MOUNT=/mnt/usb

if [ "$PROGRAM" = "" ]; then
    echo "Error, PROGRAM not selected"
    exit 1
fi

if [ ! -f build/"$PROGRAM".uf2 ]; then
    echo "Error, build/$PROGRAM.uf2 not found"
    exit 1
fi

mkdir -p $MOUNT
mount $DEVICE $MOUNT 

echo Copying $PROGRAM into $DEVICE ...
cp build/$PROGRAM.uf2 $MOUNT

# Wait a little bit
sleep 1
sync
sleep 1

echo Done!

