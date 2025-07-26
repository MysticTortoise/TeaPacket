cmake_minimum_required(VERSION 3.31)
project(TP_TGA_LOADER)

set(CMAKE_CXX_STANDARD 20)

if(NOT DEFINED TP_PLATFORM_DIR)
    set(TP_PLATFORM_DIR, Windows)
endif ()
set(TP_ASSETS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/assets)



add_executable(TP_TGA_LOADER main.cpp)
target_link_libraries(TP_TGA_LOADER PRIVATE TeaPacket)
message(${CMAKE_CURRENT_SOURCE_DIR}/assets)
TP_BuildAssets("${CMAKE_CURRENT_SOURCE_DIR}/assets" ${CMAKE_CURRENT_BINARY_DIR} TP_TGA_LOADER)