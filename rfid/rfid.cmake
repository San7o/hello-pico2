# rfid.cmake

add_executable(rfid rfid/rfid.c)

# pull in common dependencies
target_link_libraries(rfid
  pico_stdlib
  hardware_spi
)

# create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(rfid)
