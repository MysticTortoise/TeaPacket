#include "TeaPacket/Graphics/Texture.hpp"

#include <numeric>

#include "WindowsGraphics.hpp"
#include "Error/Win32ErrorHandling.hpp"
#include "TeaPacket/Memory/StructUtils.hpp"
#include "Graphics/Texture/TextureConversion.hpp"

using namespace TeaPacket::Graphics;

static constexpr D3D11_FILTER GetD3DFilter(TextureFilterType filter)
{
    switch (filter)
    {
    case TextureFilterType::POINT: return D3D11_FILTER_MIN_MAG_MIP_POINT;
    case TextureFilterType::LINEAR: return D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    }
    throw std::exception();
}

static constexpr D3D11_TEXTURE_ADDRESS_MODE GetD3DWrap(TextureWrapType wrap)
{
    switch (wrap)
    {
    case TextureWrapType::REPEAT: return D3D11_TEXTURE_ADDRESS_WRAP;
    case TextureWrapType::MIRROR: return D3D11_TEXTURE_ADDRESS_MIRROR;
    case TextureWrapType::CLAMP: return D3D11_TEXTURE_ADDRESS_CLAMP;
    }
    throw std::exception();
}

TextureFormat TeaPacket::Graphics::Pl_TexConvertToNativeFormat(unsigned char** data, unsigned short width, unsigned short height, TextureFormat format)
{
    switch (format)
    {
        using enum TextureFormat;
        // Supported formats
    case RGBA8:
    case BGRA8:
    case BGR5A1:
    case R8:
        return format;

        // Unsupported formats (will be converted)
    case RGB8:
        TEX_BPP24_TO_32(data, width, height);
        return RGBA8;

    case BGR8:
        TEX_BPP24_TO_32(data, width, height);
        return BGRA8;

    case RGB5A1:
        TEX_SWAP_RANDB(data, width, height, RGB5A1);
        return BGR5A1;
    case NONE: throw std::exception();
    }
    throw std::exception();
}

static constexpr DXGI_FORMAT GetD3DFormat(TextureFormat format)
{
    using enum TextureFormat;
    switch(format)
    {
    case RGBA8: return DXGI_FORMAT_R8G8B8A8_UNORM;
    case BGRA8: return DXGI_FORMAT_B8G8R8A8_UNORM;
    case BGR5A1: return DXGI_FORMAT_B5G5R5A1_UNORM;
    case R8: return DXGI_FORMAT_R8_UNORM;
    default:
        throw std::exception();
    }
}

void Texture::Pl_Initialize(unsigned char* data)
{

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

    std::vector<unsigned char> channelSizes = GetFormatChannelSizes(format);
    unsigned int bytesPerPixel = std::accumulate(channelSizes.begin(), channelSizes.end(), 0) / 8;
    unsigned int rowPitch = width * bytesPerPixel * sizeof(unsigned char);

    if (data != nullptr)
    {
        deviceContext->UpdateSubresource(platformTexture.texture2d.Get(), 0, NULL, data, rowPitch, 0);
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
