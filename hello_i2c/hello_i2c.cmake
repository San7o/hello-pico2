# hello_i2c.cmake

add_executable(hello_i2c hello_i2c/hello_i2c.c)

# pull in common dependencies
target_link_libraries(hello_i2c
  pico_stdlib
  hardware_i2c
)

# enable usb output, disable uart output
pico_enable_stdio_usb(hello_i2c  1)
pico_enable_stdio_uart(hello_i2c 0)

# create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(hello_i2c)
