cmake_minimum_required(VERSION 3.5.0)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY $<1:${CMAKE_SOURCE_DIR}/build/bin>)

project(exeme)
add_executable(exeme src/main.c)
set_property(TARGET exeme PROPERTY C_STANDARD 99)
