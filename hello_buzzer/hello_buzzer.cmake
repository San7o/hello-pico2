# hello_led.cmake

add_executable(hello_buzzer hello_buzzer/hello_buzzer.c)

# pull in common dependencies
target_link_libraries(hello_buzzer pico_stdlib)

# create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(hello_buzzer)
