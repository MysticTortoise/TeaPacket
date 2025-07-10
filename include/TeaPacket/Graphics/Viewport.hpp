#pragma once

#include <vector>

#include "Platform/Graphics/PlatformViewport.hpp"
#include "TeaPacket/Graphics/Texture.hpp"

namespace TeaPacket::Graphics
{

    /// Represents a singular renderable area. These can be used as textures, or blit to Displays to present to the user.
    class Viewport
    {
    public:
        unsigned short width; ///< The width of the viewport.
        unsigned short height; ///< The height of the viewport.

        PlatformViewport platformViewport; ///< Platform data.

        Texture texture; ///< The contained Texture of the viewport. May be used as a resource elsewhere.

        /// Creates a Viewport, but does not Initialize it.
        /// @param width The width of the Viewport.
        /// @param height The height of the Viewport
        Viewport(unsigned short width, unsigned short height);
        /// Initializes the Viewport using its parameters.
        void Initialize();
        /// De-Initializes the Viewport. Must be called.
        void DeInitialize();

    private:
        void Pl_Initialize();
        void Pl_DeInitialize();

    public:
        /// Creates and initializes a Viewport
        /// @param width The width of the Viewport to be created.
        /// @param height The height of the Viewport to be created.
        /// @return The Viewport that was created.
        static Viewport CreateViewport(unsigned short width, unsigned short height);

        /// A list of all existing viewports.
        inline static std::vector<Viewport> Viewports = std::vector<Viewport>();

    private:
    };
}