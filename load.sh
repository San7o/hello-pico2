#!/bin/sh

# Load a program to the Pico2

set -e

#PROGRAM=hello_led_external
#PROGRAM=hello_led
PROGRAM=hello_led_external
DEVICE=/dev/sdc1
MOUNT=/mnt/usb

mkdir -p $MOUNT
mount $DEVICE $MOUNT 

echo Copying...
cp build/$PROGRAM.uf2 $MOUNT

# Wait a little bit
sleep 1
sync
sleep 1

echo Done!

