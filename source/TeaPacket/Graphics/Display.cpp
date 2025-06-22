//
// Created by Denis on 6/16/2025.
//
#include "TeaPacket/Graphics/Display.hpp"

using namespace TeaPacket::Graphics;

Display::Display(const unsigned short width, const unsigned short height, const std::string& name):
    width(width),
    height(height),
    name(name),
    platformDisplay(),
viewport(width,height)
{
}


Display* Display::RegisterDisplay(const unsigned short width, const unsigned short height, const std::string& name)
{
    bool success;
    Displays.push_back(Pl_RegisterDisplay(width, height, name, &success));
    if(!success)
    {
        Displays.pop_back();
        return nullptr;
    }

    Displays.back().viewport = Viewport::CreateViewport(width, height);

    return &Displays.back();
}

void Display::DeInitialize()
{
    viewport.DeInitialize();
    Pl_DeInitialize();
}
