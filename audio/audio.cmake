# hello_led.cmake

add_executable(audio audio/audio.c)

# pull in common dependencies
target_link_libraries(audio pico_stdlib)

# create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(audio)
