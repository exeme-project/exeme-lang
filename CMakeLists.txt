cmake_minimum_required(VERSION 3.5.0)
project(exeme LANGUAGES C)

set(CMAKE_C_COMPILER clang)

# Find LLVM
find_package(LLVM REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")

# Collect all source files
set(SOURCES
    src/main.c
	src/errors.c
	src/globals.c
	src/args/args.c
    src/lexer/lexer.c
	src/lexer/tokens.c
    src/parser/parser.c
	src/parser/tokens.c
    src/compiler/compiler.c
	src/compiler/tokens.c
    src/utils/array.c
    src/utils/conversions.c
	src/utils/files.c
    src/utils/hashmap.c
    src/utils/panic.c
	src/utils/str.c
)

# Collect all headers (optional but helps IDEs)
set(HEADERS
	src/errors.h
	src/globals.h
	src/args/args.h
    src/lexer/lexer.h
	src/lexer/tokens.h
    src/parser/parser.h
	src/parser/tokens.h
    src/compiler/compiler.h
	src/compiler/tokens.h
    src/utils/array.h
    src/utils/conversions.h
	src/utils/files.h
    src/utils/hashmap.h
    src/utils/panic.h
	src/utils/str.h
)

# Create the executable
add_executable(exeme ${SOURCES})

# Set C standard
set_property(TARGET exeme PROPERTY C_STANDARD 99)
set_property(TARGET exeme PROPERTY C_STANDARD_REQUIRED ON)

# Include directories
target_include_directories(exeme PRIVATE
    src/lexer
    src/parser
    src/compiler
    src/utils
    ${LLVM_INCLUDE_DIRS}
)

# Compile definitions for LLVM
target_compile_definitions(exeme PRIVATE ${LLVM_DEFINITIONS})

# Set runtime output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
file(MAKE_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
