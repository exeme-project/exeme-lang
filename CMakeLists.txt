cmake_minimum_required(VERSION 3.5.0)
project(exeme LANGUAGES C)

set(CMAKE_C_COMPILER clang)

# Find LLVM
find_package(LLVM REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")

# Collect all source and header files
file(GLOB_RECURSE SOURCES "src/*.c")
file(GLOB_RECURSE HEADERS "src/*.h")

# Create the executable
add_executable(exeme ${SOURCES})

# Set C standard
set_property(TARGET exeme PROPERTY C_STANDARD 99)
set_property(TARGET exeme PROPERTY C_STANDARD_REQUIRED ON)

# Include directories
target_include_directories(exeme PRIVATE ${LLVM_INCLUDE_DIRS})

# Compile definitions for LLVM
target_compile_definitions(exeme PRIVATE ${LLVM_DEFINITIONS})

# Set runtime output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
file(MAKE_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
