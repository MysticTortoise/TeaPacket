//
// Global Interface Functions
//
#include "TeaPacket/Interface/Interface.hpp"
#include "Interface/Pl_Interface.hpp"

static bool doQuit = false;

bool TeaPacket::Interface::Initialize()
{
    if (!Pl_Initialize()) { return false; }
    initialized = true;
    return true;
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
