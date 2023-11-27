cmake_minimum_required(VERSION 3.14)

project(kinetic_lang)

# Generate compile commands for LSPs
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set C standard to C11
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 14)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

#------------------------------
# Compiler Components
#------------------------------

# Lexer
file(GLOB LEXER_SOURCES "src/lexer/*.c")
add_library(lexer ${LEXER_SOURCES})

# Parser
file(GLOB PARSER_SOURCES "src/parser/*.c")
add_library(parser ${PARSER_SOURCES})

# Code Generator
#file(GLOB GENERATOR_SOURCES "src/generator/*.c")
#add_library(generator ${GENERATOR_SOURCES})

# Main Compiler
file(GLOB COMPILER_SOURCES "src/*.c")
add_executable(kcc ${COMPILER_SOURCES})
target_link_libraries(kcc lexer parser)

# Add include directories if needed
target_include_directories(lexer PUBLIC include)
target_include_directories(parser PUBLIC include)
#target_include_directories(generator PUBLIC include)

#------------------------------
# Testing
#------------------------------

# Enable testing
enable_testing()

# Lexer Tests
file(GLOB LEXER_TEST_SOURCES "test/lexer/*.cpp")
add_executable(test_lexer ${LEXER_TEST_SOURCES})
target_link_libraries(test_lexer lexer GTest::gtest_main)

# Parser Tests
file(GLOB PARSER_TEST_SOURCES "test/parser/*.cpp")
add_executable(test_parser ${PARSER_TEST_SOURCES})
target_link_libraries(test_parser lexer parser GTest::gtest_main)

# Code Generator Tests
#file(GLOB GENERATOR_TEST_SOURCES "test/generator/*.c")
#add_executable(test_generator ${GENERATOR_TEST_SOURCES})
#target_link_libraries(test_generator lexer parser generator)
#add_test(NAME generator_test COMMAND test_generator)

include(GoogleTest)
gtest_discover_tests(test_lexer)
gtest_discover_tests(test_parser)

