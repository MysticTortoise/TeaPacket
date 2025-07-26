#pragma once

#include <vector>

#include "Platform/Assets/PlatformAssetStream.hpp"

namespace TeaPacket::Assets
{
    class AssetStream
    {
    public:
        bool endOfField;

        PlatformAssetStream platformStream;

        AssetStream(const std::string& path);
        void ReadBytes(size_t length, char* data);
        void Seek(ptrdiff_t amount, std::ios::seekdir dir);
        size_t Tell();


        void Skip(size_t length);

        char ReadByte();
        std::vector<char> ReadBytes(size_t length);

        template<typename T>
        T ReadVal()
        {
            T v;
            ReadBytes(sizeof(T), reinterpret_cast<char*>(&v));
            return v;
        }


        int8_t ReadSInt8() { return ReadVal<int8_t>(); }
        uint8_t ReadUInt8() { return ReadVal<uint8_t>(); }


        int16_t ReadSInt16LittleEndian();
        uint16_t ReadUInt16LittleEndian();
        int32_t ReadSInt32LittleEndian();
        uint32_t ReadUInt32LittleEndian();
        int64_t ReadSInt64LittleEndian();
        uint64_t ReadUInt64LittleEndian();

        int16_t ReadSInt16BigEndian();
        uint16_t ReadUInt16BigEndian();
        int32_t ReadSInt32BigEndian();
        uint32_t ReadUInt32BigEndian();
        int64_t ReadSInt64BigEndian();
        uint64_t ReadUInt64BigEndian();

    private:
        void Pl_Initialize(const std::string& path);
        void Pl_ReadBytes(size_t length, char* dest);
        void Pl_Seek(ptrdiff_t amount, std::ios::seekdir dir);
        size_t Pl_Tell();
    };
}