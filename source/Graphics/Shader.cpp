#include "TeaPacket/Graphics/Shader.hpp"

using namespace TeaPacket::Graphics;

Shader Shader::CreateShader(const std::string& vertexShaderCode, const std::string& fragmentShaderCode)
{
    Shader shader;
    shader.Initialize(vertexShaderCode, fragmentShaderCode);
    return shader;
}

void Shader::Initialize(const std::string& vertexShaderCode, const std::string& fragmentShaderCode)
{
    Pl_Initialize(vertexShaderCode, fragmentShaderCode);
}

Shader Shader::CreateShaderFromFiles(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
{
    return CreateShader(vertexShaderPath, fragmentShaderPath);
}

