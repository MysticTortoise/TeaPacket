#include "TeaPacket/Graphics/Shader.hpp"

#include "Error/Win32ErrorHandling.hpp"

#include <wrl/client.h>
#include <d3dcompiler.h>
#include <d3d10shader.h>
#include <iostream>
#include <vector>
#include <array>
#include <unordered_map>

#include "WindowsGraphics.hpp"
#include "TeaPacket/Debug/Logging.hpp"

using namespace TeaPacket::Graphics;

static const std::array<std::unordered_map<VertexAttributeType, DXGI_FORMAT>, 4> VertexAttributeTypeToDXGIFormat = {
    std::unordered_map<VertexAttributeType, DXGI_FORMAT>
    {
        // Size 1
        {VERTEX_ATTR_TYPE_FLOAT, DXGI_FORMAT_R32_FLOAT},
        {VERTEX_ATTR_TYPE_INT, DXGI_FORMAT_R32_SINT},
        {VERTEX_ATTR_TYPE_UINT, DXGI_FORMAT_R32_UINT}
    },
    {
        // Size 2
        {VERTEX_ATTR_TYPE_FLOAT, DXGI_FORMAT_R32G32_FLOAT},
        {VERTEX_ATTR_TYPE_INT, DXGI_FORMAT_R32G32_SINT},
        {VERTEX_ATTR_TYPE_UINT, DXGI_FORMAT_R32G32_UINT}
    },
    {
        // Size 3
        {VERTEX_ATTR_TYPE_FLOAT, DXGI_FORMAT_R32G32B32_FLOAT},
        {VERTEX_ATTR_TYPE_INT, DXGI_FORMAT_R32G32B32_SINT},
        {VERTEX_ATTR_TYPE_UINT, DXGI_FORMAT_R32G32B32_UINT}
    },
    {
        // Size 4
        {VERTEX_ATTR_TYPE_FLOAT, DXGI_FORMAT_R32G32B32A32_FLOAT},
        {VERTEX_ATTR_TYPE_INT, DXGI_FORMAT_R32G32B32A32_SINT},
        {VERTEX_ATTR_TYPE_UINT, DXGI_FORMAT_R32G32B32A32_UINT}
    },
};


void Shader::Pl_Initialize(const std::string& vertexShaderCode, const std::string& fragmentShaderCode, const std::vector<VertexAttribute>& inputAttributes)
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
        polygonLayout[i].Format = VertexAttributeTypeToDXGIFormat[inputAttributes[i].amount-1].at(inputAttributes[i].type);
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
}
