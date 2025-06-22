# Project Definition
cmake_minimum_required(VERSION 3.31)
project(TeaPacket)
# Overall messages
message("Building for " ${TP_PLATFORM} "...")
if(TP_DEBUG)
    message("Debug Build")
else()
    message("Release Build")
endif()
# Set the C/C++ Standards used
set(CMAKE_CXX_STANDARD 20)

# Setup source directories
set(SOURCE_DIRECTORIES source/TeaPacket/*.c* source/Platform/${TP_PLATFORM}/*.c*)

# Search for all source files
file(GLOB_RECURSE sources source/TeaPacket/*.c* source/Platform/${TP_PLATFORM}/*.c*)

# Add Library Target
add_library(TeaPacket STATIC ${sources})
# Do platform-specific CMakeLists.txt stuff.
include(buildinfo/${TP_PLATFORM}/post-app.cmake)

# Set #define s
target_compile_definitions(TeaPacket PUBLIC
        TP_PLATFORM=${TP_PLATFORM}
        TP_DEBUG=${TP_DEBUG})

# Set all include directories
target_include_directories(TeaPacket PRIVATE source)
target_include_directories(TeaPacket PUBLIC include include/${TP_PLATFORM})

# Enable warnings and treat warnings as errors.
if(MSVC)
    target_compile_options(TeaPacket PRIVATE /W4 /WX)
else()
    target_compile_options(TeaPacket PRIVATE
            -Wall -Wextra -Wpedantic -Werror
    )
endif()
# Disable extensions and require
set_target_properties(TeaPacket PROPERTIES
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

# Enable IPO if supported
if(NOT TP_DEBUG)
    set_property(TARGET TeaPacket PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()