# TeaPacket
A cross-platform framework with a focus on niche platforms.

Very pre-alpha. Do not use for any serious projects yet, as the API is *heavily* subject to change.
## Requirements
* CMake
* Python
* A valid C++20 compiler

## Dependencies
### Windows
* Windows SDK
* DirectX 11

## External Tools
TeaPacket uses a custom python script to manually handle external tools. Do not worry about downloading these.
### Windows
* [glslangValidator](https://github.com/KhronosGroup/glslang) and [SPIRV-Cross](https://github.com/KhronosGroup/SPIRV-Cross) for GLSL > HLSL Shader Compilation

## Instructions
To build, run cmake with `TP_PLATFORM_DIR` set to a directory or platform. For official platforms, simply set the platform name (eg. `Windows`). For external platforms, it must be set to the directory of the platform.

TeaPacket uses a special asset registry. To ensure all files are set up correctly, set `TP_ASSET_DIR` to a folder with all of your assets.

Documentation is available [here](https://mystictortoise.github.io/TeaPacket/index.html).

## Making your Own Platforms
Making a platform is simple. You must have:
* A CMakeLists.txt that properly calls `target_sources` and `target_include_directories` 
* A folder named `prebuild` with two folders inside: `asset_parse` and `external_tools`
  * `asset_parse` may contain python scripts which implement the `parse_and_copy` function. These parse assets specific to your platform. They must be in the format `_EXTENSION_.py`
  * `external_tools` may contain python scripts to download, compile, and save external tools needed by the build process.
* Proper implementations for all functions that begin with `Pl_`
  * Note: For functions that are not implemented yet, use the `TeaPacket::Error::FunctionNotImplemented` exception.
* Proper definitions for all `Platform___` classes needed by the framework.

It is recommended to check out the `Windows` official platform for reference.