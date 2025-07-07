#include "TeaPacket/Graphics/Shader.hpp"
#include "TeaPacket/Files/AssetReader.hpp"

using namespace TeaPacket::Graphics;

Shader Shader::CreateShader(const std::string& vertexShaderCode, const std::string& fragmentShaderCode, const std::vector<VertexAttribute>& inputAttributes)
{
    Shader shader;
    shader.Initialize(vertexShaderCode, fragmentShaderCode, inputAttributes);
    return shader;
}

void Shader::Initialize(const std::string& vertexShaderCode, const std::string& fragmentShaderCode, const std::vector<VertexAttribute>& inputAttributes)
{
    Pl_Initialize(vertexShaderCode, fragmentShaderCode, inputAttributes);
}

void Shader::UseShader()
{
    Pl_UseShader();
}

Shader Shader::CreateShaderFromFiles(const std::string& vertexShaderPath, const std::string& fragmentShaderPath, const std::vector<VertexAttribute>& inputAttributes)
{
    return CreateShader(Files::ReadTextAsset(vertexShaderPath), Files::ReadTextAsset(fragmentShaderPath), inputAttributes);
}

