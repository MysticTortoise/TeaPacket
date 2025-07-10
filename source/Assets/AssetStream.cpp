#include "TeaPacket/Assets/AssetStream.hpp"

using namespace TeaPacket::Assets;

AssetStream::AssetStream(const std::string& path)
{
    Pl_Initialize(path);
}

void AssetStream::ReadBytes(size_t length, char* data)
{
    Pl_ReadBytes(length, data);
}

void AssetStream::Seek(ptrdiff_t amount, std::ios::seekdir dir)
{
    Pl_Seek(amount, dir);
}

size_t AssetStream::Tell()
{
    return Pl_Tell();
}

std::vector<char> AssetStream::ReadBytes(const size_t length)
{
    std::vector<char> vec = std::vector<char>(length);
    ReadBytes(length, vec.data());
    return vec;
}

