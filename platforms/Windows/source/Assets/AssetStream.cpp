#include "TeaPacket/Assets/AssetStream.hpp"

#include "TeaPacket/Debug/Logging.hpp"
#include "TeaPacket/Memory/Endianness.hpp"

using namespace TeaPacket::Assets;
using namespace TeaPacket::Memory;

void AssetStream::Pl_Initialize(const std::string& path)
{
    platformStream.stream.open("assets/" + path, std::ios::binary);
}

void AssetStream::Pl_ReadBytes(size_t length, char* dest)
{
    platformStream.stream.read(dest, length);

    if (platformStream.stream.bad())
    {
        std::perror("");
        throw std::exception();
    }

    endOfField = platformStream.stream.eof();
}

void AssetStream::Pl_Seek(const ptrdiff_t amount, const std::ios::seekdir dir)
{
    platformStream.stream.seekg(amount, dir);
}

size_t AssetStream::Pl_Tell()
{
    return platformStream.stream.tellg();
}



#define READ_FUNC_U_LIL(bits) \
uint##bits##_t AssetStream::ReadUInt##bits##LittleEndian(){ \
if(IsBigEndian()) \
{ \
    return SwapEndian(ReadVal<uint##bits##_t>()); \
} \
return ReadVal<uint##bits##_t>(); \
} \

#define READ_FUNC_S_LIL(bits) \
int##bits##_t AssetStream::ReadSInt##bits##LittleEndian(){ \
if(IsBigEndian()) \
{ \
return SwapEndian(ReadVal<int##bits##_t>()); \
} \
return ReadVal<int##bits##_t>(); \
} \

#define READ_FUNC_U_BIG(bits) \
uint##bits##_t AssetStream::ReadUInt##bits##BigEndian(){ \
if(!IsBigEndian()) \
{ \
return SwapEndian(ReadVal<uint##bits##_t>()); \
} \
return ReadVal<uint##bits##_t>(); \
} \

#define READ_FUNC_S_BIG(bits) \
int##bits##_t AssetStream::ReadSInt##bits##BigEndian(){ \
if(!IsBigEndian()) \
{ \
return SwapEndian(ReadVal<int##bits##_t>()); \
} \
return ReadVal<int##bits##_t>(); \
} \

#define READ_FUNC_GEN(bits) \
READ_FUNC_U_LIL(bits) READ_FUNC_S_LIL(bits) READ_FUNC_U_BIG(bits) READ_FUNC_S_BIG(bits)

READ_FUNC_GEN(16)
READ_FUNC_GEN(32)
READ_FUNC_GEN(64)