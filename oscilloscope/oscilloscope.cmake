# oscilloscope.cmake

add_executable(oscilloscope oscilloscope/oscilloscope.c)

# pull in common dependencies
target_link_libraries(oscilloscope pico_stdlib hardware_adc hardware_dma hardware_clocks)

# enable usb output, disable uart output
pico_enable_stdio_usb(oscilloscope  1)
pico_enable_stdio_uart(oscilloscope 0)

# create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(oscilloscope)
