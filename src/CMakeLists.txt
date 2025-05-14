# CMakeLists.txt for DHT11 library
cmake_minimum_required(VERSION 3.12)

# Add library with our source files
add_library(dht11 STATIC
    dht11.c
)

# Make the library's header files available
target_include_directories(dht11 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Link to pico_stdlib which gives us core functionality
target_link_libraries(dht11 PUBLIC pico_stdlib hardware_gpio)
