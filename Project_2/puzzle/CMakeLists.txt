add_executable(puzzle
        puzzle.c
        )

# Pull in our pico_stdlib which pulls in commonly used features
target_link_libraries(puzzle pico_stdlib hardware_adc)

pico_enable_stdio_usb(puzzle 1)
pico_enable_stdio_uart(puzzle 0)
# create map/bin/hex file etc.
pico_add_extra_outputs(puzzle)

# add url via pico_set_program_url
example_auto_set_url(puzzle)