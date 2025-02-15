cmake_minimum_required(VERSION 3.5.0)

# Set project and language
project(exeme LANGUAGES C)

set(CMAKE_C_COMPILER clang)

# Create the executable
add_executable(exeme src/main.c)
set_property(TARGET exeme PROPERTY C_STANDARD 99)
set_property(TARGET exeme PROPERTY C_STANDARD_REQUIRED ON)

# Find LLVM
find_package(LLVM REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")

# Include LLVM headers and definitions for the target
target_include_directories(exeme PRIVATE ${LLVM_INCLUDE_DIRS})
target_compile_definitions(exeme PRIVATE ${LLVM_DEFINITIONS})

# Link against LLVM libraries (only C components)
target_link_libraries(exeme LLVM)

# Set runtime output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/bin)
file(MAKE_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
