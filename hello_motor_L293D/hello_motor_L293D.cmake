# hello_motor_L293D.cmake

add_executable(hello_motor_L293D hello_motor_L293D/hello_motor_L293D.c)

# pull in common dependencies
target_link_libraries(hello_motor_L293D
  pico_stdlib
  hardware_adc
  hardware_pwm
)

# enable usb output, disable uart output
pico_enable_stdio_usb(hello_motor_L293D 1)
pico_enable_stdio_uart(hello_motor_L293D 0)

# create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(hello_motor_L293D)
