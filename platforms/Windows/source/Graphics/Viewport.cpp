#include "TeaPacket/Graphics/Viewport.hpp"
#include "Graphics/WindowsGraphics.hpp"
#include "Error/Win32ErrorHandling.hpp"

using namespace TeaPacket::Graphics;

void Viewport::Pl_Initialize()
{

    texture.height = height;
    texture.width = width;
    texture.filterType = TextureFilterType::POINT;
    texture.wrapType = TextureWrapType::CLAMP;

    texture.Initialize(nullptr);

    D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
    D3D11_TEXTURE2D_DESC texDesc;
    texture.platformTexture.texture2d->GetDesc(&texDesc);
    renderTargetViewDesc.Format = texDesc.Format;
    renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
    renderTargetViewDesc.Texture2D.MipSlice = 0;

    CheckErrorWinCom(
        device->CreateRenderTargetView(texture.platformTexture.texture2d.Get(), &renderTargetViewDesc, platformViewport.renderTargetView.ReleaseAndGetAddressOf())
        );

    D3D11_TEXTURE2D_DESC depthBufferDesc;
    ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

    depthBufferDesc.Width = width;
    depthBufferDesc.Height = height;
    depthBufferDesc.MipLevels = 1;
    depthBufferDesc.ArraySize = 1;
    depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthBufferDesc.SampleDesc.Count = 1;
    depthBufferDesc.SampleDesc.Quality = 0;
    depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthBufferDesc.CPUAccessFlags = 0;
    depthBufferDesc.MiscFlags = 0;

    CheckErrorWinCom(
        device->CreateTexture2D(&depthBufferDesc, NULL, platformViewport.depthTexture.ReleaseAndGetAddressOf())
    );

    D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
    ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

    depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    depthStencilViewDesc.Texture2D.MipSlice = 0;

    CheckErrorWinCom(
        device->CreateDepthStencilView(platformViewport.depthTexture.Get(), &depthStencilViewDesc, platformViewport.depthStencilView.ReleaseAndGetAddressOf())
    );

    platformViewport.d3dviewport.Width = static_cast<float>(width);
    platformViewport.d3dviewport.Height = static_cast<float>(height);
    platformViewport.d3dviewport.MinDepth = 0;
    platformViewport.d3dviewport.MaxDepth = 1;
    platformViewport.d3dviewport.TopLeftX = 0;
    platformViewport.d3dviewport.TopLeftY = 0;
}

void Viewport::Pl_DeInitialize()
{

}

