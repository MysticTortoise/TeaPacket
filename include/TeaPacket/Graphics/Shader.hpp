#pragma once
#include <string>

namespace TeaPacket::Graphics
{
    class Shader
    {
    public:
        void Initialize(const std::string& vertexShaderCode, const std::string& fragmentShaderCode);
    private:
        void Pl_Initialize(const std::string& vertexShaderCode, const std::string& fragmentShaderCode);
    public:
        static Shader CreateShader(const std::string& vertexShaderCode, const std::string& fragmentShaderCode);
        static Shader CreateShaderFromFiles(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
    private:

    };
}
