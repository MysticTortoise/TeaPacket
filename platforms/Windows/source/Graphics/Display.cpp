//
// Display implementation file for Windows using Win32
//
#include "../Interface/WindowsInterface.hpp"
#include "TeaPacket/Graphics/Display.hpp"
#include "Graphics/WindowsGraphics.hpp"
#include "Error/Win32ErrorHandling.hpp"
#include "TeaPacket/Math/Vector2.hpp"

#include "TeaPacket/Graphics/Model.hpp"
#include "TeaPacket/Graphics/Shader.hpp"
#include "TeaPacket/Graphics/Texture.hpp"

#include <windows.h>
#include <iostream>
#include <wrl/client.h>
#include <dxgi1_3.h>

#include "TeaPacket/Debug/Logging.hpp"
#include "TeaPacket/Memory/StructUtils.hpp"


using namespace TeaPacket::Graphics;

static Vector2l GetFullDesktopResolution()
{
    RECT desktop;
    GetWindowRect(GetDesktopWindow(), &desktop);
    return Vector2l{desktop.right, desktop.bottom};
}

static Vector2l GetDesktopResolution()
{
    return Vector2l{GetSystemMetrics(SM_CXFULLSCREEN), GetSystemMetrics(SM_CYFULLSCREEN)};
}

static const std::vector<char> FullscreenMesh = {
    '\x00', '\x00', '\x80', '\x3f', '\x00', '\x00', '\x80', '\x3f', '\x00', '\x00', '\x80', '\x3f', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x80', '\x3f', '\x00', '\x00', '\x80', '\xbf', '\x00', '\x00', '\x80', '\x3f', '\x00', '\x00', '\x80', '\x3f', '\x00', '\x00', '\x80', '\xbf', '\x00', '\x00', '\x80', '\xbf', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x80', '\x3f', '\x00', '\x00', '\x80', '\xbf', '\x00', '\x00', '\x80', '\x3f', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00'
};
static const std::vector<unsigned long> FullscreenIndices = {0, 1, 3, 1, 2, 3};
static Model fullscreenModel;
static Shader fullscreenShader;
static Texture hackyTex;

static const std::string vShader = R"(static float4 gl_Position;
static float2 o_uv;
static float2 i_uv;
static float2 i_position;

struct SPIRV_Cross_Input
{
    float2 i_position : TEXCOORD0;
    float2 i_uv : TEXCOORD1;
};

struct SPIRV_Cross_Output
{
    float2 o_uv : TEXCOORD0;
    float4 gl_Position : SV_Position;
};

void vert_main()
{
    o_uv = i_uv;
    gl_Position = float4(i_position, 0.0f, 1.0f);
}

SPIRV_Cross_Output main(SPIRV_Cross_Input stage_input)
{
    i_uv = stage_input.i_uv;
    i_position = stage_input.i_position;
    vert_main();
    SPIRV_Cross_Output stage_output;
    stage_output.gl_Position = gl_Position;
    stage_output.o_uv = o_uv;
    return stage_output;
}
)";
static const std::string pShader = R"(Texture2D<float4> u_mainTex : register(t0);
SamplerState _u_mainTex_sampler : register(s0);

static float4 o_color;
static float2 i_uv;

struct SPIRV_Cross_Input
{
    float2 i_uv : TEXCOORD0;
};

struct SPIRV_Cross_Output
{
    float4 o_color : SV_Target0;
};

void frag_main()
{
    o_color = u_mainTex.Sample(_u_mainTex_sampler, i_uv);
}

SPIRV_Cross_Output main(SPIRV_Cross_Input stage_input)
{
    i_uv = stage_input.i_uv;
    frag_main();
    SPIRV_Cross_Output stage_output;
    stage_output.o_color = o_color;
    return stage_output;
}
)";


static void BlitterInit()
{
    static bool inited = false;
    if (inited)
    {
        return;
    }
    inited = true;

    fullscreenModel = Model::CreateModel(FullscreenMesh, FullscreenIndices, {ShaderVariableType(SHADER_VAR_TYPE_FLOAT,2), ShaderVariableType(SHADER_VAR_TYPE_FLOAT, 2)});
    fullscreenShader = Shader::CreateShader(vShader, pShader, {ShaderVariableType(SHADER_VAR_TYPE_FLOAT,2), ShaderVariableType(SHADER_VAR_TYPE_FLOAT, 2)}, {ShaderVariableType(SHADER_VAR_TYPE_TEXTURE,1)});

    D3D11_SAMPLER_DESC samplerDesc =
        {D3D11_FILTER_MIN_MAG_MIP_POINT, D3D11_TEXTURE_ADDRESS_CLAMP, D3D11_TEXTURE_ADDRESS_CLAMP, D3D11_TEXTURE_ADDRESS_CLAMP,
        0, 1, D3D11_COMPARISON_ALWAYS, {0,0,0,0}, 0, D3D11_FLOAT32_MAX};
    device->CreateSamplerState(&samplerDesc, hackyTex.platformTexture.samplerState.GetAddressOf());

}

Display Display::Pl_RegisterDisplay(const unsigned short width, const unsigned short height, const std::string& name, bool* success)
{
    Display display = Display(width, height, name);

    RECT rect;
    Vector2l screenSize = {1280, 720};
    SetRect(&rect, 0, 0, screenSize.x(), screenSize.y());
    //AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);
    display.platformDisplay.windowHandle = CreateWindow(
        "TeaPacket_MainWindowClass",
        name.c_str(),
        WS_OVERLAPPEDWINDOW,
        0, 0,
        rect.right - rect.left, rect.bottom - rect.top,
        0,
        0,
        Interface::Platform::instanceHandle,
        0
    );

    if (display.platformDisplay.windowHandle == NULL)
    {
        DWORD dwError = GetLastError();
        if (dwError != ERROR_CLASS_ALREADY_EXISTS)
        {
            CheckErrorWinCom(HRESULT_FROM_WIN32(dwError));
        }
    }
    ShowWindow(display.platformDisplay.windowHandle, SW_SHOWNORMAL);

    DXGI_SWAP_CHAIN_DESC desc;
    ZeroStruct(desc);
    desc.Windowed = TRUE;
    desc.BufferCount = 2;
    desc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    desc.BufferDesc.Width = rect.right - rect.left;
    desc.BufferDesc.Height = rect.bottom - rect.top;
    desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    desc.OutputWindow = display.platformDisplay.windowHandle;
    desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

    Microsoft::WRL::ComPtr<IDXGIDevice3> dxgiDevice;
    CheckErrorWinCom(device.As(&dxgiDevice));

    Microsoft::WRL::ComPtr<IDXGIAdapter> adapter;
    Microsoft::WRL::ComPtr<IDXGIFactory> factory;

    CheckErrorWinCom(dxgiDevice->GetAdapter(&adapter));
    CheckErrorWinCom(adapter->GetParent(IID_PPV_ARGS(&factory)));
    CheckErrorWinCom(
        factory->CreateSwapChain(device.Get(), &desc, &display.platformDisplay.swapchain)
        );

    Microsoft::WRL::ComPtr<ID3D11Texture2D> backbuffer;
    CheckErrorWinCom(
        display.platformDisplay.swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<LPVOID*>(backbuffer.GetAddressOf()))
    );

    CheckErrorWinCom(
        device->CreateRenderTargetView(
            backbuffer.Get(),
            NULL, display.platformDisplay.renderTargetView.GetAddressOf())
        );

    D3D11_TEXTURE2D_DESC depthBufferDesc;
    ZeroStruct(depthBufferDesc);
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
        device->CreateTexture2D(&depthBufferDesc, NULL, display.platformDisplay.depthStencilBuffer.GetAddressOf())
    );

    D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
    ZeroStruct(depthStencilDesc);

    depthStencilDesc.DepthEnable = true;
    depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

    depthStencilDesc.StencilEnable = true;
    depthStencilDesc.StencilReadMask = 0xFF;
    depthStencilDesc.StencilWriteMask = 0xFF;

    depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
    depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
    depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
    depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    CheckErrorWinCom(
        device->CreateDepthStencilState(&depthStencilDesc, display.platformDisplay.depthStencilState.GetAddressOf())
    );
    deviceContext->OMSetDepthStencilState(display.platformDisplay.depthStencilState.Get(), 1);

    D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
    ZeroStruct(depthStencilViewDesc);

    depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    depthStencilViewDesc.Texture2D.MipSlice = 0;

    CheckErrorWinCom(
        device->CreateDepthStencilView(display.platformDisplay.depthStencilBuffer.Get(), &depthStencilViewDesc, display.platformDisplay.depthStencilView.GetAddressOf())
    );

    display.platformDisplay.d3dviewport.Width = width;
    display.platformDisplay.d3dviewport.Height = height;
    display.platformDisplay.d3dviewport.MinDepth = 0;
    display.platformDisplay.d3dviewport.MaxDepth = 1;
    display.platformDisplay.d3dviewport.TopLeftX = 0;
    display.platformDisplay.d3dviewport.TopLeftY = 0;

    BlitterInit();

    *success = true;
    return display; 
}

bool Display::Pl_Resize(const unsigned short newWidth, const unsigned short newHeight)
{
    (void)newWidth;
    (void)newHeight;
    return true;
}

void Display::Pl_RenderViewport(const Viewport& viewport)
{

    deviceContext->ClearDepthStencilView(platformDisplay.depthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);

    viewport.platformViewport.texture.As(&hackyTex.platformTexture.texture2d);
    viewport.platformViewport.shaderResourceView.As(&hackyTex.platformTexture.textureView);

    deviceContext->OMSetRenderTargets(1, platformDisplay.renderTargetView.GetAddressOf(), platformDisplay.depthStencilView.Get());
    deviceContext->RSSetViewports(1, &platformDisplay.d3dviewport);

    ShaderVariableValue val;
    val.val_tex = &hackyTex;
    fullscreenShader.SetParameter(0, val);

    fullscreenShader.UseShader();
    fullscreenModel.Render();

    platformDisplay.swapchain->Present(0, 0);
}


void Display::Pl_DeInitialize()
{
    platformDisplay.swapchain->SetFullscreenState(FALSE, NULL);
}