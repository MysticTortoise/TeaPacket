# TeaPacket Windows Platform

TeaPacket for Windows platforms.

Currently uses:
* Modern Windows API for system functions.
  * Older versions of the Windows API are untested. Older versions may be explicitly supported at a later date.
* Direct3D 11 for rendering.
  * Different rendering backends may become available in the future (OpenGL, Vulkan, Direct3D 12)
  * Older versions of Direct3D may become available in the future.

## Dependencies
* Windows SDK
* DirectX 11

## External Tools
* [glslangValidator](https://github.com/KhronosGroup/glslang) and [SPIRV-Cross](https://github.com/KhronosGroup/SPIRV-Cross) for GLSL > HLSL Shader Compilation