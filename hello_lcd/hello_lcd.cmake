# hello_lcd.cmake

add_executable(hello_lcd hello_lcd/hello_lcd.c)

# pull in common dependencies
target_link_libraries(hello_lcd
  pico_stdlib
  hardware_i2c
)

# enable usb output, disable uart output
pico_enable_stdio_usb(hello_lcd  1)
pico_enable_stdio_uart(hello_lcd 0)

# create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(hello_lcd)
