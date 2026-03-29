# hello_led.cmake

add_executable(audio
  audio/audio.c
  audio/audio_data.S
)

configure_file(audio/audio_data.raw audio/audio_data.raw COPYONLY)

# pull in common dependencies
target_link_libraries(audio
  pico_stdlib
  pico_audio_i2s
)

# create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(audio)
