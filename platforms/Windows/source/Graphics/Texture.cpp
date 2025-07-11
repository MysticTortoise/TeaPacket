#include "TeaPacket/Graphics/Texture.hpp"

#include "WindowsGraphics.hpp"
#include "../Error/Win32ErrorHandling.hpp"
#include "TeaPacket/Memory/StructUtils.hpp"

using namespace TeaPacket::Graphics;

static constexpr D3D11_FILTER GetD3DFilter(TextureFilterType filter)
{
    switch (filter)
    {
    case TEXTURE_FILTER_POINT: return D3D11_FILTER_MIN_MAG_MIP_POINT;
    case TEXTURE_FILTER_LINEAR: return D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    }
    throw std::exception();
}

static constexpr D3D11_TEXTURE_ADDRESS_MODE GetD3DWrap(TextureWrapType wrap)
{
    switch (wrap)
    {
    case TEXTURE_WRAP_REPEAT: return D3D11_TEXTURE_ADDRESS_WRAP;
    case TEXTURE_WRAP_MIRROR: return D3D11_TEXTURE_ADDRESS_MIRROR;
    case TEXTURE_WRAP_CLAMP: return D3D11_TEXTURE_ADDRESS_CLAMP;
    }
    throw std::exception();
}

static constexpr DXGI_FORMAT GetD3DFormat(TextureFormat format)
{
    switch(format)
    {
    case TEXTURE_FORMAT_RGBA8: return DXGI_FORMAT_R8G8B8A8_UNORM;
    case TEXTURE_FORMAT_RGB8: return DXGI_FORMAT_R8G8B8A8_UNORM;
    }
    throw std::exception();
}

/// Converts 24 bpp textures to 32bpp textures because FUCK MICROSOFT \n
/// Seriously they got rid of 24bpp textures in Direct3D 11 WHYYYYYYY
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
    if (GetFormatChannelSizes(format).size() == 3)
    {
        data = FUCK_MICROSOFT(data, width, height);
    }

    D3D11_TEXTURE2D_DESC textureDesc;
    textureDesc.Height = height;
    textureDesc.Width = width;
    textureDesc.MipLevels = 0;
    textureDesc.ArraySize = 1;
    textureDesc.Format = GetD3DFormat(format);
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
        if (GetFormatChannelSizes(format).size() == 3)
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

    samplerDesc.Filter = GetD3DFilter(filterType);
    D3D11_TEXTURE_ADDRESS_MODE d3dWrapType = GetD3DWrap(wrapType);
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
