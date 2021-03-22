add_executable(sha1
        sha1.c
        )

# Pull in our pico_stdlib which pulls in commonly used features
target_link_libraries(sha1 pico_stdlib hardware_adc)

pico_enable_stdio_usb(sha1 1)
# create map/bin/hex file etc.
pico_add_extra_outputs(sha1)

# add url via pico_set_program_url
auto_set_url(sha1)