# hello_uart.cmake

add_executable(hello_uart hello_uart/hello_uart.c)

# pull in common dependencies
target_link_libraries(hello_uart pico_stdlib)

# enable uart output, disable uart output
pico_enable_stdio_uart(hello_uart 1)
pico_enable_stdio_uart(hello_uart 0)

# create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(hello_uart)
