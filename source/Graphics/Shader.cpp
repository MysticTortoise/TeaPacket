#include "TeaPacket/Graphics/Shader.hpp"
#include "TeaPacket/Files/AssetReader.hpp"
#include "TeaPacket/Memory/StructUtils.hpp"

using namespace TeaPacket::Graphics;

Shader Shader::CreateShader(const std::string& vertexShaderCode, const std::string& fragmentShaderCode, const std::vector<ShaderVariableType>& inputAttributes, const std::vector<ShaderVariableType>& uniforms)
{
    Shader shader;
    shader.Initialize(vertexShaderCode, fragmentShaderCode, inputAttributes, uniforms);
    return shader;
}

void Shader::Initialize(const std::string& vertexShaderCode, const std::string& fragmentShaderCode, const std::vector<ShaderVariableType>& inputAttributes, const std::vector<ShaderVariableType>& uniforms)
{
    Pl_Initialize(vertexShaderCode, fragmentShaderCode, inputAttributes);
    for (const ShaderVariableType& shaderVariable : uniforms)
    {
        ShaderVariable var;
        ZeroStruct(var);
        var.type = shaderVariable;
        shaderUniforms.push_back(var);
    }
}

void Shader::UseShader()
{
    Pl_UseShader();
}

void Shader::SetParameter(unsigned char index, ShaderVariableValue value)
{
    shaderUniforms[index].value = value;
}

Shader Shader::CreateShaderFromFiles(const std::string& vertexShaderPath, const std::string& fragmentShaderPath, const std::vector<ShaderVariableType>& inputAttributes, const std::vector<ShaderVariableType>& uniforms)
{
    return CreateShader(Files::ReadTextAsset(vertexShaderPath), Files::ReadTextAsset(fragmentShaderPath), inputAttributes, uniforms);
}

