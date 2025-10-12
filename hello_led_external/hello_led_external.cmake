# hello_led.cmake

add_executable(hello_led_external hello_led_external/hello_led_external.c)

# pull in common dependencies
target_link_libraries(hello_led_external pico_stdlib)

# create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(hello_led_external)
