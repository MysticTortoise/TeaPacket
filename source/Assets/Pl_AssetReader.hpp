#pragma once
#include <vector>
#include <string>
namespace TeaPacket::Assets
{
    bool Pl_Initialize();
    void Pl_DeInitialize();

    std::vector<char> Pl_ReadAllBytesFromAsset(const std::string& asset, bool isText = false);
}
