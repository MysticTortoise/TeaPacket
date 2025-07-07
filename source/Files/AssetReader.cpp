#include "TeaPacket/Files/AssetReader.hpp"
#include "Files/Pl_AssetReader.hpp"

using namespace TeaPacket::Files;

std::string TeaPacket::Files::ReadTextAsset(const std::string& path)
{
    std::vector<char> vec = Pl_ReadAllBytesFromAsset(path, false);
    if (vec.back() == '\0')
    {
        vec.pop_back();
    }
    return std::string(vec.begin(), vec.end());
}

std::vector<char> TeaPacket::Files::ReadAllBytesFromAsset(const std::string& path)
{
    return Pl_ReadAllBytesFromAsset(path);
}
