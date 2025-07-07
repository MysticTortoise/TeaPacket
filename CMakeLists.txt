# Project Definition
cmake_minimum_required(VERSION 3.31)
project(TeaPacket)

option(TEAPACKET_BUILD_EXAMPLES "Build example programs")

if(NOT DEFINED TP_PLATFORM_DIR)
    set(TP_PLATFORM_DIR Windows)
endif ()
STRING(REGEX REPLACE "\\\\" "/" TP_PLATFORM_DIR ${TP_PLATFORM_DIR})
if(NOT "${TP_PLATFORM_DIR}" MATCHES "/")
    set(TP_PLATFORM_DIR ${CMAKE_CURRENT_SOURCE_DIR}/platforms/${TP_PLATFORM_DIR}/)
endif()

# Overall messages
message("Building for " ${TP_PLATFORM_DIR} "...")
if(TP_DEBUG)
    message("Debug Build")
else()
    message("Release Build")
endif()
# Set the C/C++ Standards used
set(CMAKE_CXX_STANDARD 20)

# Search for all source files
file(GLOB_RECURSE sources source/*.c*)

# Add Library Target
add_library(TeaPacket STATIC ${sources})
add_subdirectory(${TP_PLATFORM_DIR})

# Set #define s
target_compile_definitions(TeaPacket PUBLIC
        TP_PLATFORM=${TP_PLATFORM}
        TP_DEBUG=${TP_DEBUG})

# Set all include directories
target_include_directories(TeaPacket PRIVATE source)
target_include_directories(TeaPacket PUBLIC include)

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
    message("IPO ON")
    set_property(TARGET TeaPacket PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

set(TEAPACKET_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

function(TP_BuildAssets tp_asset_path build_path)
    add_custom_target(
            py_prebuild ALL
            COMMAND python ${TEAPACKET_SOURCE_DIR}/prebuild/prebuild.py ${TEAPACKET_SOURCE_DIR} ${TP_PLATFORM_DIR} ${tp_asset_path} ${build_path}
            COMMENT "Running prebuild script..."
            USES_TERMINAL
    )
    add_dependencies(TeaPacket py_prebuild)
endfunction()

if (DEFINED TP_ASSET_DIR})
    message("AUTOBUILD")
    TP_BuildAssets(${TP_ASSET_DIR} ${CMAKE_BINARY_DIR})
endif ()


if(${TEAPACKET_BUILD_EXAMPLES})
    add_subdirectory(examples)
endif ()