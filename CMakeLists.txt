cmake_minimum_required(VERSION 3.21)
project("search_engine" VERSION 0.1)

set(M_I_D ${CMAKE_SOURCE_DIR}/include/)
set(M_S_D ${CMAKE_SOURCE_DIR}/src/)

set(CMAKE_CXX_STANDARD 17)

add_executable(
        search_engine
        src/main.cpp
        ${M_I_D}ConverterJSON.h
        ${M_S_D}ConverterJSON.cpp
        ${M_I_D}InvertedIndex.h
        ${M_S_D}InvertedIndex.cpp
        include/SearchServer.h
        src/SearchServer.cpp
        include/RelativeIndex.h)

## nlohmann_json

include(FetchContent)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.10.5/json.tar.xz)
FetchContent_MakeAvailable(json)

target_include_directories(search_engine PUBLIC ${M_I_D})

target_link_libraries(search_engine PRIVATE nlohmann_json::nlohmann_json)

add_subdirectory(test)