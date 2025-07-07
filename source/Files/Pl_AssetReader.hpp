#pragma once
#include <vector>
#include <string>
namespace TeaPacket::Files
{
    std::vector<char> Pl_ReadAllBytesFromAsset(const std::string& asset, bool isText = false);
}
