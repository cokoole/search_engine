cmake_minimum_required(VERSION 3.21)
project("search_engine" VERSION 0.1)

set(M_I_D ${CMAKE_SOURCE_DIR}/include/)
set(M_S_D ${CMAKE_SOURCE_DIR}/src/)

set(CMAKE_CXX_STANDARD 17)

add_executable(search_engine src/main.cpp ${M_I_D}ConverterJSON.h ${M_S_D}ConverterJSON.cpp)

### GTest
#enable_testing()
#
#include(FetchContent)
#FetchContent_Declare(
#        googletest
#        URL https://github.com/google/googletest/archive/e2239ee6043f73722e7aa812a459f54a28552929.zip
#)
#
#set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
#FetchContent_MakeAvailable(googletest)
#
#include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
#
#set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
#FetchContent_MakeAvailable(googletest)
#
#add_test(search_engine example_test COMMAND example)
#include(GoogleTest)
#gtest_discover_tests(search_engine)

## nlohmann_json

include(FetchContent)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.10.5/json.tar.xz)
FetchContent_MakeAvailable(json)

target_include_directories(search_engine PUBLIC ${M_I_D})

## target_link_libraries nlohmann_json and gtest_main

target_link_libraries(search_engine PRIVATE nlohmann_json::nlohmann_json)

add_subdirectory(test)