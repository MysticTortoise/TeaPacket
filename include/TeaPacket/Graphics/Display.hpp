#pragma once
#include <string>
#include <vector>
#include "../../../platforms/Windows/include/Platform/Graphics/PlatformDisplay.hpp"
#include "TeaPacket/Graphics/Viewport.hpp"


namespace TeaPacket::Graphics
{
    /// Represents a singular viewable area.
    /** On Desktop platforms, there can be any number of displays, each representing one Window on the OS. \n
        On non-desktop platforms, this is typically only one display representing the screen. \n
        Some specific platforms may have a fixed number that is more than 1, if the system has, for example, a second screen.
    **/
    class Display
    {
    public:
        unsigned short width;   ///< Width of the Display in pixels
        unsigned short height;  ///< Height of the Display in pixels
        std::string name; ///< Name of the display. Will be shown to the user on platforms that allow.

        PlatformDisplay platformDisplay; ///< Private platform-dependent data.

        bool Resize(unsigned short newWidth, unsigned short newHeight);

        void RenderViewport(const Viewport& viewport);

        /// De-initializes the Display and its graphics.
        void DeInitialize();

    private:
        /// Creates (but does not initialize) a Display. Use Display::CreateDisplay instead.
        /** This function exists to populate the variables of the Display object. **/
        Display(unsigned short width, unsigned short height, const std::string& name);

        bool Pl_Resize(unsigned short newWidth, unsigned short newHeight);
        void Pl_RenderViewport(const Viewport& viewport);

        /// Platform-Specific De-Initialize function.
        void Pl_DeInitialize();
    public:
        /// A list of all displays in existence. Displays MUST be registered to this list upon creation.
        inline static std::vector<Display> Displays = std::vector<Display>();

        /// Registers a new display.
        /** Has different effects depending on the platform. \n
         * On desktop platforms, this will create a new window. \n
         * On platforms with multiple screens, this must be called to utilize them.
         * (Ex. on Wii U, this must be called to register the Gamepad screen.) \n
         * On any other platforms, it will return nullptr
         * @param width The width of the display. Only matters on desktop platforms.
         * @param height The height of the display. Only matters on desktop platforms.
         * @param name The name of the display. Only matters on desktop platforms.
         * @return The Display that was created. Will be nullptr if creating a display isn't possible.
         * **/
        static Display* RegisterDisplay(unsigned short width = 0, unsigned short height = 0, const std::string& name = "Window");

    private:
        /// Platform-specific Display registration.
        static Display Pl_RegisterDisplay(unsigned short width, unsigned short height, const std::string& name, bool* success);
    };
}
