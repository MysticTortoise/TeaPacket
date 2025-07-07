#pragma once
#include <string>
#include <vector>

namespace TeaPacket::Files
{
    std::string ReadTextAsset(const std::string& path);
    std::vector<char> ReadAllBytesFromAsset(const std::string& path);
}
