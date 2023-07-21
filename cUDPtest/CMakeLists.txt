cmake_minimum_required(VERSION 3.0)
project(UDPListener)

set(CMAKE_C_STANDARD 11)

add_executable(udp_listener main.c)

if(WIN32)
    target_link_libraries(udp_listener PRIVATE ws2_32)
endif()