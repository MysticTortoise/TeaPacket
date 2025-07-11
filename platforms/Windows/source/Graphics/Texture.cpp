#include "TeaPacket/Graphics/Texture.hpp"

#include <unordered_map>

#include "WindowsGraphics.hpp"
#include "../Error/Win32ErrorHandling.hpp"
#include "TeaPacket/Memory/StructUtils.hpp"

using namespace TeaPacket::Graphics;


static const std::unordered_map<TextureFilterType, D3D11_FILTER> d3dFilterMap = {
    {TEXTURE_FILTER_POINT, D3D11_FILTER_MIN_MAG_MIP_POINT},
    {TEXTURE_FILTER_LINEAR, D3D11_FILTER_MIN_MAG_MIP_LINEAR}
};

static const std::unordered_map<TextureWrapType, D3D11_TEXTURE_ADDRESS_MODE> d3dWrapMap = {
    {TEXTURE_WRAP_REPEAT, D3D11_TEXTURE_ADDRESS_WRAP},
    {TEXTURE_WRAP_MIRROR, D3D11_TEXTURE_ADDRESS_MIRROR},
    {TEXTURE_WRAP_CLAMP, D3D11_TEXTURE_ADDRESS_CLAMP}
};

static const std::unordered_map<TextureFormat, DXGI_FORMAT> d3dFormatMap = {
    {TEXTURE_FORMAT_RGBA8, DXGI_FORMAT_R8G8B8A8_UNORM},
    {TEXTURE_FORMAT_RGB8, DXGI_FORMAT_R8G8B8A8_UNORM}
};

static unsigned char* FUCK_MICROSOFT(const unsigned char* data, unsigned short width, unsigned short height)
{
    size_t src = 0;
    size_t dst = 0;
    unsigned char* newData = new unsigned char[width * height * 4];
    for (unsigned short y = 0; y < height; y++)
    {
        for (unsigned short x = 0; x < width; x++)
        {
            memcpy(&newData[dst], &data[src], 3);
            newData[dst+3] = 0;
            dst += 4;
            src += 3;
        }
    }
    return newData;
}

void Texture::Pl_Initialize(const unsigned char* data)
{
    if (GetFormatInfo(format).channelCount == 3)
    {
        data = FUCK_MICROSOFT(data, width, height);
    }

    D3D11_TEXTURE2D_DESC textureDesc;
    textureDesc.Height = height;
    textureDesc.Width = width;
    textureDesc.MipLevels = 0;
    textureDesc.ArraySize = 1;
    textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    textureDesc.SampleDesc.Count = 1;
    textureDesc.SampleDesc.Quality = 0;
    textureDesc.Usage = D3D11_USAGE_DEFAULT;
    textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
    textureDesc.CPUAccessFlags = 0;
    textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

    CheckErrorWinCom(
        device->CreateTexture2D(&textureDesc, NULL, platformTexture.texture2d.ReleaseAndGetAddressOf())
    );

    unsigned int rowPitch = width * 4 * sizeof(unsigned char);

    if (data != nullptr)
    {
        deviceContext->UpdateSubresource(platformTexture.texture2d.Get(), 0, NULL, data, rowPitch, 0);
        if (GetFormatInfo(format).channelCount == 3)
        {
            delete[] data;
        }
    }

    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
    srvDesc.Format = textureDesc.Format;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MostDetailedMip = 0;
    srvDesc.Texture2D.MipLevels = UINT_MAX;

    CheckErrorWinCom(
        device->CreateShaderResourceView(platformTexture.texture2d.Get(), &srvDesc, platformTexture.textureView.ReleaseAndGetAddressOf())
    );


    D3D11_SAMPLER_DESC samplerDesc;

    samplerDesc.Filter = d3dFilterMap.at(filterType);
    D3D11_TEXTURE_ADDRESS_MODE d3dWrapType = d3dWrapMap.at(wrapType);
    samplerDesc.AddressU = d3dWrapType;
    samplerDesc.AddressV = d3dWrapType;
    samplerDesc.AddressW = d3dWrapType;
    samplerDesc.MipLODBias = 0;
    samplerDesc.MaxAnisotropy = 1;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
    ZeroStruct(samplerDesc.BorderColor);
    samplerDesc.MinLOD = 0;
    samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

    CheckErrorWinCom(
        device->CreateSamplerState(&samplerDesc, platformTexture.samplerState.ReleaseAndGetAddressOf())
    );

}
