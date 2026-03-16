# hello_lcd.cmake

add_executable(hello_lcd hello_lcd/hello_lcd.c)

# pull in common dependencies
target_link_libraries(hello_lcd pico_stdlib)

# create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(hello_lcd)
