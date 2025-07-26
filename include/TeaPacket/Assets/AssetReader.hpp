#pragma once
#include <string>
#include <vector>

/// Functionality relating to files
namespace TeaPacket::Assets
{
    bool Initialize();
    inline bool initialized = false;
    void DeInitialize();

    /// Read an asset from the Asset Registry in plain text form.
    std::string ReadTextAsset(const std::string& path);
    /// Read the raw bytes from an Asset in the Asset Registry.
    std::vector<char> ReadAllBytesFromAsset(const std::string& path);
}
