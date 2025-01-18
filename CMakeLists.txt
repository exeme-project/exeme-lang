cmake_minimum_required(VERSION 3.5.0)

# Set the compiler to Clang
set(CMAKE_C_COMPILER "clang")
set(CMAKE_CXX_COMPILER "clang++")

# Specify the project name and target
project(exeme)
add_executable(exeme src/main.c)
set_property(TARGET exeme PROPERTY C_STANDARD 99)

# Find LLVM
find_package(LLVM REQUIRED CONFIG)

# Include LLVM directories and definitions
include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

# Link LLVM libraries to your project
llvm_map_components_to_libnames(LLVM_LIBS Core BitWriter)
target_link_libraries(exeme ${LLVM_LIBS})

# Set the runtime output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/bin)
