//
// Global Interface Functions
//
#include "TeaPacket/Interface/Interface.hpp"
#include "TeaPacket/Interface/Pl_Interface.hpp"

static bool doQuit = false;

void TeaPacket::Interface::Initialize()
{
    Pl_Initialize();
}

void TeaPacket::Interface::Tick()
{
    Pl_Tick();
}

void TeaPacket::Interface::DeInitialize()
{
    Pl_DeInitialize();
}

bool TeaPacket::Interface::ShouldQuit()
{
    return doQuit;
}

void TeaPacket::Interface::QueueQuit()
{
    doQuit = true;
}
