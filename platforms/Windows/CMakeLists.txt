set(TP_PLATFORM Windows)

file(GLOB_RECURSE sources_win source/*.c*)
target_sources(TeaPacket PRIVATE ${sources_win})
target_include_directories(TeaPacket PUBLIC include)
target_include_directories(TeaPacket PRIVATE source)

target_link_libraries(TeaPacket d3d11.lib D3DCompiler.lib)