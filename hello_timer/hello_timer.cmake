# hello_timer.cmake

add_executable(hello_timer hello_timer/hello_timer.c)

# pull in common dependencies
target_link_libraries(hello_timer pico_stdlib)

# enable usb output, disable uart output
pico_enable_stdio_usb(hello_timer  1)
pico_enable_stdio_uart(hello_timer 0)

# create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(hello_timer)
