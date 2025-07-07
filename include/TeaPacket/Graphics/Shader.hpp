#pragma once
#include <string>
#include <vector>
#include "Platform/Graphics/PlatformShader.hpp"

#include "TeaPacket/Graphics/Model.hpp"

namespace TeaPacket::Graphics
{
    class Shader
    {
    public:

        PlatformShader platformShader;

        void Initialize(const std::string& vertexShaderCode, const std::string& fragmentShaderCode, const std::vector<VertexAttribute>& inputAttributes);
        void UseShader();
    private:
        void Pl_Initialize(const std::string& vertexShaderCode, const std::string& fragmentShaderCode, const std::vector<VertexAttribute>& inputAttributes);
        void Pl_UseShader();
    public:
        static Shader CreateShader(const std::string& vertexShaderCode, const std::string& fragmentShaderCode, const std::vector<VertexAttribute>& inputAttributes);
        static Shader CreateShaderFromFiles(const std::string& vertexShaderPath, const std::string& fragmentShaderPath, const std::vector<VertexAttribute>& inputAttributes);
    private:

    };
}
