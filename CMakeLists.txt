# CMakeLists.txt for DHT11 example project
cmake_minimum_required(VERSION 3.12)
set(PICO_BOARD pico_w)
# Include the SDK
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

# Project name
project(dht11_example C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the SDK
pico_sdk_init()

# Add our library directory
add_subdirectory(src)


