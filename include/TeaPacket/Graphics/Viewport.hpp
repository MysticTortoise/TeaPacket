#pragma once

#include "Platform/Graphics/PlatformViewport.hpp"

namespace TeaPacket::Graphics
{
    class Display;

    class Viewport
    {
    public:
        unsigned short width;
        unsigned short height;

        PlatformViewport platformViewport;

        Viewport(unsigned short width, unsigned short height);
        void DeInitialize();

    private:
        Pl_DeInitialize();

    public:
        static Viewport CreateViewport(unsigned short width, unsigned short height, Display* display = nullptr);

    private:
        static Viewport Pl_CreateViewport(unsigned short width, unsigned short height, Display* display);
    };
}