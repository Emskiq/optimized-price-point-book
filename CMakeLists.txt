cmake_minimum_required(VERSION 3.14)
project(OrderBooks LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

set(CMAKE_CXX_FLAGS_DEBUG "-g -O0" CACHE STRING "" FORCE)
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG" CACHE STRING "" FORCE)

# include_directories(src)
file(GLOB LIB_SRCS src/*.cpp)

add_library(orderbooks_lib ${LIB_SRCS})
target_include_directories(orderbooks_lib PRIVATE ${CMAKE_SOURCE_DIR})

# add_executable(${PROJECT_NAME} main.cpp)
# target_link_libraries(${PROJECT_NAME} PRIVATE orderbooks_lib)

# Executable 2: Benchmark
add_executable(benchmark bin/Benchmark.cpp)
target_compile_options(benchmark PRIVATE -O2)
target_link_libraries(benchmark PRIVATE orderbooks_lib)

# Executable 3: Generation of the data
add_executable(generate_data bin/GenerateData.cpp)

# Enable testing
include(CTest)

# Fetch GoogleTest
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/release-1.12.1.tar.gz
)
FetchContent_MakeAvailable(googletest)

# Test executable
add_executable(tests tests/testOrderBook.cpp)
target_link_libraries(tests PRIVATE gtest_main orderbooks_lib)
target_include_directories(tests PRIVATE ${CMAKE_SOURCE_DIR})
add_test(NAME unit_tests COMMAND tests)
