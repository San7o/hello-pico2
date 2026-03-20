# hello_adc.cmake

add_executable(hello_adc hello_adc/hello_adc.c)

# pull in common dependencies
target_link_libraries(hello_adc pico_stdlib hardware_adc)

# enable usb output, disable uart output
pico_enable_stdio_usb(hello_adc  1)
pico_enable_stdio_uart(hello_adc 0)

# create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(hello_adc)
