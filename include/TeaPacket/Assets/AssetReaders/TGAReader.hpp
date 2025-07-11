#pragma once

#include "TeaPacket/Graphics/Texture.hpp"
#include <string>

namespace TeaPacket::Assets
{
    Graphics::Texture ReadTGA(const std::string& path);
}