#pragma once
#include <string>
#include <vector>
#include "Platform/Graphics/PlatformShader.hpp"
#include "TeaPacket/Graphics/ShaderVariable.hpp"

namespace TeaPacket::Graphics
{

    /// A combined shader (or program) in the rendering pipeline. Used to interpret and render Model data.
    class Shader
    {
    public:
        /// Platform data.
        PlatformShader platformShader;

        /// Initialize a shader
        /// @param vertexShaderCode The code for the vertex shader to be used.
        /// @param fragmentShaderCode The code for the fragment (pixel) shader to be used.
        /// @param inputAttributes The VertexAttributes that the vertexShader expects. Should be the same as whatever Models you use.
        void Initialize(const std::string& vertexShaderCode, const std::string& fragmentShaderCode,
                        const std::vector<ShaderVariableType>& inputAttributes, const std::vector<ShaderVariableType>& uniforms);
        /// Bind the shader for rendering. Every Model drawn after this call to UseShader() will use this shader until a new shader is bound.
        void UseShader();

        void SetParameter(unsigned char index, ShaderVariableValue value);
    private:
        std::vector<ShaderVariable> shaderUniforms = {};

        void Pl_Initialize(const std::string& vertexShaderCode, const std::string& fragmentShaderCode, const std::vector<ShaderVariableType>& inputAttributes);
        void Pl_UseShader();
    public:
        /// Creates a shader. See Initialize() for parameter info.
        /// @return The shader created.
        static Shader CreateShader(const std::string& vertexShaderCode, const std::string& fragmentShaderCode, const std::vector<ShaderVariableType>&
                                   inputAttributes, const std::vector<ShaderVariableType>& uniforms);
        /// Load a shader from the Asset Registry
        /// @param vertexShaderPath The path to the .vert file to load vertex shader code from.
        /// @param fragmentShaderPath The path to the .frag file to load fragment shader code from.
        /// @param inputAttributes The VertexAttributes that the vertexShader expects. Should be the same as whatever Models you use.
        /// @return The shader created.
        static Shader CreateShaderFromFiles(const std::string& vertexShaderPath, const std::string& fragmentShaderPath,
                                            const std::vector<ShaderVariableType>& inputAttributes, const std::vector<ShaderVariableType>& uniforms);
    private:

    };
}
