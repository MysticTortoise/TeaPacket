//
// Created by Denis on 6/16/2025.
//
#include "TeaPacket/Graphics/Display.hpp"

using namespace TeaPacket::Graphics;

Display::Display(const unsigned short width, const unsigned short height, const std::string& name):
    width(width),
    height(height),
    name(name)
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
    Display& display = Displays.back();
    return &display;
}

bool Display::Resize(const unsigned short newWidth, const unsigned short newHeight)
{
    if (Pl_Resize(newWidth, newHeight))
    {
        this->width = newWidth;
        this->height = newHeight;
        return true;
    }
    return false;
}


void Display::DeInitialize()
{
    Pl_DeInitialize();
}
