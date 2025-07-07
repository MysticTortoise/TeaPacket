#pragma once

#include <d3d11.h>
#include "wrl/client.h"

namespace TeaPacket::Graphics
{
    Microsoft::WRL::ComPtr<ID3D11Device> device;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext;
}