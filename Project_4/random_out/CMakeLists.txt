add_executable(random_out
        random_out.c
        )

# Pull in our pico_stdlib which pulls in commonly used features
target_link_libraries(random_out pico_stdlib hardware_adc)

pico_enable_stdio_usb(random_out 1)
# create map/bin/hex file etc.
pico_add_extra_outputs(random_out)

# add url via pico_set_program_url
auto_set_url(random_out)