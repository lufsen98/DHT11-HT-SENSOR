# CMakeLists.txt for DHT11 library

# Add library with our source files
add_library(dht11 STATIC
    dht11.c
)

# Make the library's header files available
target_include_directories(dht11 PUBLIC ${CMAKE_SOURCE_DIR}/include)
# Link to pico_stdlib which gives us core functionality
target_link_libraries(dht11 PUBLIC 
    pico_stdlib 
    hardware_gpio
)

# Add executable
add_executable(dht11_example main.c)  

# Enable USB output, disable UART output
pico_enable_stdio_usb(dht11_example 1)
pico_enable_stdio_uart(dht11_example 0)

# Create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(dht11_example)

# Link to libraries
target_link_libraries(dht11_example 
    pico_stdlib
    dht11
)
