cmake_minimum_required(VERSION 3.5)
project(kinetic_lang C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_C_FLAGS "-Wall -Wextra -Werror -g")
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS}")

# Get GoogleTest
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        release-1.12.1
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

file(GLOB_RECURSE SOURCES src/*.c src/*.h)

add_library(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

file(GLOB_RECURSE EXAMPLE_SOURCES example/*.c)
add_executable(${PROJECT_NAME}_example ${EXAMPLE_SOURCES})
target_link_libraries(${PROJECT_NAME}_example ${PROJECT_NAME})

## Testing
enable_testing()

file(GLOB_RECURSE TEST_SOURCES test/*.cpp)
add_executable(${PROJECT_NAME}_test ${TEST_SOURCES})
target_link_libraries(${PROJECT_NAME}_test ${PROJECT_NAME} gtest_main)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME}_test)

