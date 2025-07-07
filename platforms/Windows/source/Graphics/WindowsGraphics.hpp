#pragma once

#include <d3d11.h>
#include "wrl/client.h"

namespace TeaPacket::Graphics
{
    extern Microsoft::WRL::ComPtr<ID3D11Device> device;
    extern Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext;
}