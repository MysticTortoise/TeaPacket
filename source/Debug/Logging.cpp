#include "TeaPacket/Debug/Logging.hpp"
#include "Debug/Pl_Logging.hpp"

void TeaPacket::Print(const std::string& message)
{
    Pl_Print(message);
}

