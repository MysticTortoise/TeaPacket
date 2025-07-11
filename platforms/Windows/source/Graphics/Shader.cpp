#include "TeaPacket/Graphics/Shader.hpp"

#include "Error/Win32ErrorHandling.hpp"

#include <wrl/client.h>
#include <d3dcompiler.h>
#include <d3d10shader.h>
#include <iostream>
#include <vector>
#include <array>

#include "WindowsGraphics.hpp"
#include "TeaPacket/Debug/Logging.hpp"
#include "TeaPacket/Graphics/ShaderVariable.hpp"
#include "TeaPacket/Graphics/Texture.hpp"

using namespace TeaPacket::Graphics;

static constexpr DXGI_FORMAT GetDXGIFormatFromVertexAttribute(ShaderVariableType shaderVarType)
{
    switch (shaderVarType.amount)
    {
    case 1:
        switch (shaderVarType.type)
        {
        case SHADER_VAR_TYPE_FLOAT: return DXGI_FORMAT_R32_FLOAT;
        case SHADER_VAR_TYPE_INT: return DXGI_FORMAT_R32_SINT;
        case SHADER_VAR_TYPE_UINT: return DXGI_FORMAT_R32_UINT;
        case SHADER_VAR_TYPE_TEXTURE: return DXGI_FORMAT_UNKNOWN;
        }
    case 2:
        switch (shaderVarType.type)
        {
        case SHADER_VAR_TYPE_FLOAT: return DXGI_FORMAT_R32G32_FLOAT;
        case SHADER_VAR_TYPE_INT: return DXGI_FORMAT_R32G32_SINT;
        case SHADER_VAR_TYPE_UINT: return DXGI_FORMAT_R32G32_UINT;
        case SHADER_VAR_TYPE_TEXTURE: return DXGI_FORMAT_UNKNOWN;
        }
    case 3:
        switch (shaderVarType.type)
        {
        case SHADER_VAR_TYPE_FLOAT: return DXGI_FORMAT_R32G32B32_FLOAT;
        case SHADER_VAR_TYPE_INT: return DXGI_FORMAT_R32G32B32_SINT;
        case SHADER_VAR_TYPE_UINT: return DXGI_FORMAT_R32G32B32_UINT;
        case SHADER_VAR_TYPE_TEXTURE: return DXGI_FORMAT_UNKNOWN;
        }
    case 4:
        switch (shaderVarType.type)
        {
        case SHADER_VAR_TYPE_FLOAT: return DXGI_FORMAT_R32G32B32A32_FLOAT;
        case SHADER_VAR_TYPE_INT: return DXGI_FORMAT_R32G32B32A32_SINT;
        case SHADER_VAR_TYPE_UINT: return DXGI_FORMAT_R32G32B32A32_UINT;
        case SHADER_VAR_TYPE_TEXTURE: return DXGI_FORMAT_UNKNOWN;
        }
    }
    throw std::exception();
}


void Shader::Pl_Initialize(const std::string& vertexShaderCode, const std::string& fragmentShaderCode, const std::vector<ShaderVariableType>& inputAttributes)
{
    Microsoft::WRL::ComPtr<ID3D10Blob> errorMessage;
    Microsoft::WRL::ComPtr<ID3D10Blob> vertexShaderBuffer;

    HRESULT result =
        D3DCompile(vertexShaderCode.c_str(),vertexShaderCode.length(),
            NULL, NULL, NULL,
            "main", "vs_5_0",
            D3D10_SHADER_ENABLE_STRICTNESS, 0,
            vertexShaderBuffer.GetAddressOf(), errorMessage.GetAddressOf());

    if (FAILED(result))
    {
        if (errorMessage.Get())
        {
            const char* compileErrors = static_cast<char*>(errorMessage->GetBufferPointer());
            unsigned long long bufferSize = errorMessage->GetBufferSize();
            for (unsigned long long i = 0; i < bufferSize; i++)
            {
                Print((char)compileErrors[i]);
            }
            PrintLine("");
            std::cout << std::endl;
            throw std::runtime_error("SHADER FAILED TO COMPILE");
        }
    }

    Microsoft::WRL::ComPtr<ID3D10Blob> pixelShaderBuffer;

    result =
        D3DCompile(fragmentShaderCode.c_str(),fragmentShaderCode.length(),
            NULL, NULL, NULL,
            "main", "ps_5_0",
            D3D10_SHADER_ENABLE_STRICTNESS, 0,
            pixelShaderBuffer.GetAddressOf(), errorMessage.GetAddressOf());

    if (FAILED(result))
    {
        if (errorMessage.Get())
        {
            const char* compileErrors = static_cast<char*>(errorMessage->GetBufferPointer());
            unsigned long long bufferSize = errorMessage->GetBufferSize();
            for (unsigned long long i = 0; i < bufferSize; i++)
            {
                Print((char)compileErrors[i]);
            }
            PrintLine("");
            std::cout << std::endl;
            throw std::runtime_error("SHADER FAILED TO COMPILE");
        }
    }

    CheckErrorWinCom(
        device->CreateVertexShader(
            vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(),
            NULL, platformShader.vertexShader.GetAddressOf())
    );
    CheckErrorWinCom(
        device->CreatePixelShader(
            pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(),
            NULL, platformShader.pixelShader.GetAddressOf())
    );

    std::vector<D3D11_INPUT_ELEMENT_DESC> polygonLayout = std::vector<D3D11_INPUT_ELEMENT_DESC>(inputAttributes.size());

    for (size_t i = 0; i < inputAttributes.size(); i++)
    {
        polygonLayout[i].SemanticName = "TEXCOORD";
        polygonLayout[i].SemanticIndex = static_cast<unsigned int>(i);
        polygonLayout[i].Format = GetDXGIFormatFromVertexAttribute(inputAttributes[i]);
        polygonLayout[i].InputSlot = 0;
        polygonLayout[i].AlignedByteOffset = i == 0 ? 0 : D3D11_APPEND_ALIGNED_ELEMENT;
        polygonLayout[i].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
        polygonLayout[i].InstanceDataStepRate = 0;
    }

    unsigned int numElements = static_cast<unsigned int>(inputAttributes.size());
    CheckErrorWinCom(
        device->CreateInputLayout(polygonLayout.data(), numElements,
            vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(),
            platformShader.inputLayout.GetAddressOf())
    );

}

void Shader::Pl_UseShader()
{
    deviceContext->IASetInputLayout(platformShader.inputLayout.Get());
    deviceContext->VSSetShader(platformShader.vertexShader.Get(), NULL, 0);
    deviceContext->PSSetShader(platformShader.pixelShader.Get(), NULL, 0);

    for (size_t i = 0; i < shaderUniforms.size(); i++)
    {
        const ShaderVariable& uniform = shaderUniforms[i];

        switch (uniform.type.type)
        {
        case SHADER_VAR_TYPE_TEXTURE:
            deviceContext->PSSetShaderResources(static_cast<unsigned int>(i), 1, uniform.value.tex->platformTexture.textureView.GetAddressOf());
            deviceContext->PSSetSamplers(static_cast<unsigned int>(i), 1, uniform.value.tex->platformTexture.samplerState.GetAddressOf());
            break;
        default:
            break;
        }
    }
}