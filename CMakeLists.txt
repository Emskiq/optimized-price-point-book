cmake_minimum_required(VERSION 3.14)
project(OrderBooks LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

set(CMAKE_CXX_FLAGS_DEBUG "-g -O0" CACHE STRING "" FORCE)
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG" CACHE STRING "" FORCE)

find_package(OpenSSL REQUIRED)

include(FetchContent)

FetchContent_Declare(
    boost
    URL https://boostorg.jfrog.io/artifactory/main/release/1.87.0/source/boost_1_87_0.tar.gz
)
FetchContent_MakeAvailable(boost)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/release-1.12.1.tar.gz
)
FetchContent_MakeAvailable(googletest)

include_directories(${boost_SOURCE_DIR})

# include_directories(src)
file(GLOB LIB_SRCS src/*.cpp)

add_library(orderbooks_lib ${LIB_SRCS})
target_include_directories(orderbooks_lib PRIVATE ${CMAKE_SOURCE_DIR})

# add_executable(${PROJECT_NAME} main.cpp)
# target_link_libraries(${PROJECT_NAME} PRIVATE orderbooks_lib)

add_executable(benchmark bin/Benchmark.cpp)
target_compile_options(benchmark PRIVATE -O2)
target_link_libraries(benchmark PRIVATE orderbooks_lib)

add_executable(generate_data bin/GenerateData.cpp)

# Binance Listener Executable
add_executable(binance_listener
    src/BinanceWebSocketListener.cpp
    bin/BinanceListener.cpp
)

# Link Boost and OpenSSL for Binance Listener
target_link_libraries(binance_listener
    OpenSSL::SSL
)

# Enable testing
include(CTest)

# Test executable
add_executable(tests tests/testOrderBook.cpp)
target_link_libraries(tests PRIVATE gtest_main orderbooks_lib)
target_include_directories(tests PRIVATE ${CMAKE_SOURCE_DIR})
add_test(NAME unit_tests COMMAND tests)
