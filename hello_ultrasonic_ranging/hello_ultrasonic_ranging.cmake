# hello_ultrasonic_ranging.cmake

add_executable(hello_ultrasonic_ranging hello_ultrasonic_ranging/hello_ultrasonic_ranging.c)

# enable usb output, disable uart output
pico_enable_stdio_usb(hello_ultrasonic_ranging  1)
pico_enable_stdio_uart(hello_ultrasonic_ranging 0)

# pull in common dependencies
target_link_libraries(hello_ultrasonic_ranging pico_stdlib)

# create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(hello_ultrasonic_ranging)
