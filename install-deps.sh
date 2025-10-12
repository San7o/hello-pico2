#!/bin/sh

set -e

DEPS="cmake python3 make gcc gcc-c++ arm-none-eabi-gcc-cs arm-none-eabi-gcc-cs-c++ arm-none-eabi-newlib openocd minicom"


echo Installing $DEPS
sudo dnf install $DEPS
