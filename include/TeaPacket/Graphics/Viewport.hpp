#pragma once

#include <vector>

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
        void Initialize();
        void DeInitialize();

    private:
        void Pl_Initialize();
        void Pl_DeInitialize();

    public:
        static Viewport CreateViewport(unsigned short width, unsigned short height);

        inline static std::vector<Viewport> Viewports = std::vector<Viewport>();

    private:
    };
}