#include <filesystem>

#include "Assets/Pl_AssetReader.hpp"

#include <fstream>
#include <exception>

std::vector<char> TeaPacket::Assets::Pl_ReadAllBytesFromAsset(const std::string& asset, const bool isText)
{
    std::ios_base::openmode openmode = std::ios::in;
    if (isText)
    {
        openmode |= std::ios::binary;
    }
    std::ifstream stream("assets/" + asset, openmode);
    if (!stream)
    {
        throw std::runtime_error("File " + asset + " failed to open.");
    }
    stream.unsetf(std::ios::skipws);

    stream.seekg(0, std::ios::end);
    const std::streampos size = stream.tellg();
    stream.seekg(0, std::ios::beg);

    std::vector<char> vec;
    vec.reserve(size);

    vec.insert(vec.begin(), std::istream_iterator<char>(stream), std::istream_iterator<char>());
    return vec;
}
