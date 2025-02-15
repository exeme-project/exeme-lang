cmake_minimum_required(VERSION 3.5.0)

# Set project and language
project(exeme LANGUAGES C)

# Force the use of clang as the compiler
set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)

# Create a static library for the source files
add_library(exeme_lib STATIC
    src/args/args.c
    src/compiler/compiler.c
    src/errors.c
    src/globals.c
    src/lexer/lexer.c
    src/parser/parser.c
    src/utils/array.c
    src/utils/conversions.c
    src/utils/hashmap.c
    src/utils/panic.c
    src/utils/string.c
)

# Create the executable
add_executable(exeme src/main.c)
set_property(TARGET exeme PROPERTY C_STANDARD 99)
set_property(TARGET exeme PROPERTY C_STANDARD_REQUIRED ON)

# Link the static library to the executable
target_link_libraries(exeme exeme_lib)

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
