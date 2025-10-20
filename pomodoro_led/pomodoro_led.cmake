# pomodoro_led.cmake

add_executable(pomodoro_led pomodoro_led/pomodoro_led.c)

# pull in common dependencies
target_link_libraries(pomodoro_led pico_stdlib)

# create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(pomodoro_led)
