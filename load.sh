#!/bin/sh

# Load a program to the Pico2

set -e

#PROGRAM=hello_led
#PROGRAM=hello_usb
#PROGRAM=hello_led_external
#PROGRAM=pomodoro_led
PROGRAM=hello_uart
DEVICE=/dev/sdc1
MOUNT=/mnt/usb

mkdir -p $MOUNT
mount $DEVICE $MOUNT 

echo Copying $PROGRAM into $DEVICE ...
cp build/$PROGRAM.uf2 $MOUNT

# Wait a little bit
sleep 1
sync
sleep 1

echo Done!

