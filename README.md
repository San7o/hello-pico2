# hello-pico2

![pico-img](./pi-banner.jpg)

Collection of examples on the Raspberry Pi Pico2.

You may be also interested in:

- [hello-stm32-blue-pill](https://github.com/San7o/hello-stm32-blue-pill):
  example projects for the STM32 "Blue Pill"
- [hello-esp32](https://github.com/San7o/hello-esp32):  Example projects for the
  ESP32 (private for now)
- [hello-milkv-duo-s](https://github.com/San7o/hello-milkv-duo-s): Example
  projects for the MilkV Duo S (private for now)
- [rpi3B-os](https://github.com/San7o/rpi3b-os): an operating system for the
  Rasbperry Pi 3B

## Info

- Dual Cortex-M33 or Hazard3 processors at 150MHz
- 520 kB on-chip SRAM
- 2 on-chip PLLs for internal or external clock generation
- 2 UARTS
- 2 I2C controllers
- 24 PWM channels
- USB 1.1 controller
- 12 PIO state machines
- 1 HSTX periphera;
- 2 ADC
- SPI

## Usage

Download:

```bash
git clone --recurse-submodules https://github.com/San7o/hello-pico2.git
```

or

```
git submodule update --init
```

To build the program, use:

```bash
cmake -Bbuild && cmake --build build
```

In order to load the program, you need to do the following steps.

- plug the Pi to your laptop while holding the button on the Pi, you should
  see it being detected in dmesg
- the operating system should have registered the Pi as a device in /dev,
  check this with `lsblk` and note which device is being used
- run `load.sh` (with sudo priviledges if necessary)

### Reset button hack

Instead of unplugging and plugging back the USB connection to the Pi,
you can reboot the board by connecting the RUN pin to ground. To make
this easier you can add a button that closes that circuit and reboots
the board. Then the full sequence looks like this:

1. press and hold your new reset button
2. press and hold the white BOOTSEL button on the Pico2
3. release the reset button
4. release the BOOTSEL button

## Resources

- [Pi Pico2 Datasheet](https://datasheets.raspberrypi.com/pico/pico-2-datasheet.pdf) (for the GPIO voltage)
- [RP2350 Datasheet](https://pip-assets.raspberrypi.com/categories/1214-rp2350/documents/RP-008373-DS-2-rp2350-datasheet.pdf?disposition=inline)
- [Pico SDK](https://datasheets.raspberrypi.com/pico/raspberry-pi-pico-c-sdk.pdf)
- [Pico 2 Examples](https://github.com/raspberrypi/pico-examples)
- [Kit Guide](https://docs.freenove.com/projects/fnk0058/en/latest/index.html)
