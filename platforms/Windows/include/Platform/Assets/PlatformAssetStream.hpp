#pragma once

#include <fstream>

namespace TeaPacket::Assets
{
    namespace Platform
    {
        class WindowsAssetStream
        {
        public:
            std::ifstream stream;
        };
    }
    typedef Platform::WindowsAssetStream PlatformAssetStream;
}