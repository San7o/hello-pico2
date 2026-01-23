#!/bin/sh

set -e

DEPS_FEDORA="cmake python3 make gcc gcc-c++ arm-none-eabi-gcc-cs arm-none-eabi-gcc-cs-c++ arm-none-eabi-newlib openocd minicom"
DEPS_DEBIAN="cmake python3 make gcc g++ gcc-arm-none-eabi libnewlib-arm-none-eabi libstdc++-arm-none-eabi-newlib openocd minicom"

echo Installing "$DEPS"

DISTRO=$(uname -v | awk '{print $4}')

case "$DISTRO" in
    Debian)
        sudo apt install $DEPS_DEBIAN
        ;;
    Fedora)
        sudo dnf install $DEPS_FEDORA
        ;;
esac
