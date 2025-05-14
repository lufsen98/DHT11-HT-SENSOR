# CMakeLists.txt for DHT11 example project
cmake_minimum_required(VERSION 3.12)

# Include the SDK
include(pico_sdk_import.cmake)

# Project name
project(dht11_example C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the SDK
pico_sdk_init()

# Add our library directory
add_subdirectory(lib/dht11)

# Add executable
add_executable(dht11_example
    dht11_example.c
)

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
