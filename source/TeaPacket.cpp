//
// Core engine functions implementation.
//
#include "TeaPacket/TeaPacket.hpp"

#include "TeaPacket/Interface/Interface.hpp"
#include "TeaPacket/Graphics/Graphics.hpp"

bool TeaPacket::Initialize()
{
    if (!Interface::Initialize()) { return false; }
    Interface::initialized = true;

    if (!Graphics::Initialize()) { return false; }
    Graphics::initialized = true;

    return true;
}

bool TeaPacket::ShouldRun()
{
    return !Interface::ShouldQuit();
}

void TeaPacket::Tick()
{
    Interface::Tick();
}

void TeaPacket::DeInitialize()
{
    Graphics::DeInitialize();
    Graphics::initialized = false;

    Interface::DeInitialize();
    Interface::initialized = false;
}

void TeaPacket::RunEngine()
{
    if (!Initialize())
    {
        DeInitialize();
        return;
    }
    while(ShouldRun())
    {
        Tick();
    }
    DeInitialize();
}

