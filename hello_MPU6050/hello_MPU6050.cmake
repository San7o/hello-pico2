# hello_MPU6050.cmake

add_executable(hello_MPU6050 hello_MPU6050/hello_MPU6050.c)

# pull in common dependencies
target_link_libraries(hello_MPU6050
  pico_stdlib
  hardware_i2c
)

# enable usb output, disable uart output
pico_enable_stdio_usb(hello_MPU6050 1)
pico_enable_stdio_uart(hello_MPU6050 0)

# create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(hello_MPU6050)
