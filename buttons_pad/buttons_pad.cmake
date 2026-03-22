# buttons_pad.cmake

add_executable(buttons_pad buttons_pad/buttons_pad.c)

# pull in common dependencies
target_link_libraries(buttons_pad
  pico_stdlib
)

# enable usb output, disable uart output
pico_enable_stdio_usb(buttons_pad  1)
pico_enable_stdio_uart(buttons_pad 0)

# create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(buttons_pad)
