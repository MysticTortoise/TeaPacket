/** @file DebugLevels.hpp
*/
#pragma once

#define TP_DEBUG_REALLY_UNSAFE -2 ///< Disables all safety checks. Not recommended as this will crash frequently due to many uncontrollable external factors.
#define TP_DEBUG_UNSAFE -1 ///< Disables most safety checks. Well written code should avoid any crashes here, but any bugs in your code will cause errors in TeaPacket instead of simply erroring.
#define TP_DEBUG_NONE 0 ///< Disables all debug features. Should be used for most release builds where efficiency is important.
#define TP_DEBUG_LOGGING_ONLY 1 ///< Disables all debug features other than logging. Usable for release builds in case you want users to be able to report errors.
#define TP_DEBUG_STANDARD 2 ///< Standard debug mode. Builds the game with debug enabled.
#define TP_DEBUG_MOST 9999 ///< Enables all debug features.