cmake_minimum_required(VERSION 3.21)

project(testSearchEngine)

set(CMAKE_CXX_STANDARD 17)

add_executable(testSearchEngine
        testSearchEngine.cpp
        ../include/InvertedIndex.h
        ../src/InvertedIndex.cpp
        ../include/ConverterJSON.h
        ../src/ConverterJSON.cpp
        ../include/SearchServer.h
        ../src/SearchServer.cpp ../include/RelativeIndex.h)

include_directories(../include
        ../src
        )

## GTest
enable_testing()

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/e2239ee6043f73722e7aa812a459f54a28552929.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_test(testSearchEngine example_test COMMAND example)
include(GoogleTest)
gtest_discover_tests(testSearchEngine)

## target_link_libraries gtest_main

include(FetchContent)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.10.5/json.tar.xz)
FetchContent_MakeAvailable(json)

target_include_directories(testSearchEngine PUBLIC ${M_I_D})

target_link_libraries(testSearchEngine PRIVATE gtest_main nlohmann_json::nlohmann_json)
