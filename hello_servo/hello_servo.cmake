# hello_servo.cmake

add_executable(hello_servo hello_servo/hello_servo.c)

# pull in common dependencies
target_link_libraries(hello_servo
  pico_stdlib
  hardware_pwm
)


# enable usb output, disable uart output
pico_enable_stdio_usb(hello_servo 1)
pico_enable_stdio_uart(hello_servo 0)

# create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(hello_servo)
