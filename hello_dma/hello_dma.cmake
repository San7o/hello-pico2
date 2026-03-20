# hello_dma.cmake

add_executable(hello_dma hello_dma/hello_dma.c)

# pull in common dependencies
target_link_libraries(hello_dma pico_stdlib hardware_dma)

# enable usb output, disable uart output
pico_enable_stdio_usb(hello_dma 1)
pico_enable_stdio_uart(hello_dma 0)

# create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(hello_dma)
