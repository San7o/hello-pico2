# hello-pico2

![pico-img](./pi-banner.jpg)

Collection of examples on the Raspberry Pi Pico2.

## Usage

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
- [Pico SDK](https://datasheets.raspberrypi.com/pico/raspberry-pi-pico-c-sdk.pdf)
- [Pico 2 Examples](https://github.com/raspberrypi/pico-examples)
- [Kit Guide](https://docs.freenove.com/projects/fnk0058/en/latest/index.html)
