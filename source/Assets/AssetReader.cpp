#include "TeaPacket/Assets/AssetReader.hpp"
#include "Assets/Pl_AssetReader.hpp"

using namespace TeaPacket::Assets;

bool TeaPacket::Assets::Initialize()
{
    return Pl_Initialize();
}

void TeaPacket::Assets::DeInitialize()
{
    Pl_DeInitialize();
}


std::string TeaPacket::Assets::ReadTextAsset(const std::string& path)
{
    std::vector<char> vec = Pl_ReadAllBytesFromAsset(path, false);
    if (vec.back() == '\0')
    {
        vec.pop_back();
    }
    return std::string(vec.begin(), vec.end());
}

std::vector<char> TeaPacket::Assets::ReadAllBytesFromAsset(const std::string& path)
{
    return Pl_ReadAllBytesFromAsset(path);
}
