# servo_control.cmake

add_executable(servo_control servo_control/servo_control.c)

# pull in common dependencies
target_link_libraries(servo_control pico_stdlib
  hardware_adc
  hardware_clocks
  hardware_pwm
)

# enable usb output, disable uart output
pico_enable_stdio_usb(servo_control  1)
pico_enable_stdio_uart(servo_control 0)

# create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(servo_control)
