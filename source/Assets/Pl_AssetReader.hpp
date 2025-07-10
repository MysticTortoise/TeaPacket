#pragma once
#include <vector>
#include <string>
namespace TeaPacket::Assets
{
    std::vector<char> Pl_ReadAllBytesFromAsset(const std::string& asset, bool isText = false);
}
