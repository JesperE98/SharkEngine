#ifndef ENGINE_CORE_UTILITIES_COLOR_H
#define ENGINE_CORE_UTILITIES_COLOR_H

namespace Shark::Core {

	// FColor struct to represent Color Codes
	struct FColor {

		const char* Code;

		// Constructor to allow "static constexpr FColor Red = 12"
		constexpr FColor(const char* InCode) : Code(InCode) {}

		// Windows Console Color Codes
		static const FColor Black, DarkBlue, DarkGreen, DarkCyan, DarkRed,
							DarkMagenta, DarkYellow, LightGrey, DarkGrey,
							Blue, Green, Cyan, Red, Magenta, Yellow, White, Reset;
	};

    // ANSI Escape Sequences: \033[ is the "Escape" + [1;XXm defines the color
    inline constexpr FColor FColor::Reset =         "\033[0m";
    inline constexpr FColor FColor::Black =         "\033[1;30m";
    inline constexpr FColor FColor::DarkRed =       "\033[0;31m";
    inline constexpr FColor FColor::Red =           "\033[1;31m";
    inline constexpr FColor FColor::DarkGreen =     "\033[0;32m";
    inline constexpr FColor FColor::Green =         "\033[1;32m";
    inline constexpr FColor FColor::DarkYellow =    "\033[0;33m";
    inline constexpr FColor FColor::Yellow =        "\033[1;33m";
    inline constexpr FColor FColor::DarkBlue =      "\033[0;34m";
    inline constexpr FColor FColor::Blue =          "\033[1;34m";
    inline constexpr FColor FColor::DarkMagenta =   "\033[0;35m";
    inline constexpr FColor FColor::Magenta =       "\033[1;35m";
    inline constexpr FColor FColor::DarkCyan =      "\033[0;36m";
    inline constexpr FColor FColor::Cyan =          "\033[1;36m";
    inline constexpr FColor FColor::DarkGrey =      "\033[1;90m";
    inline constexpr FColor FColor::LightGrey =     "\033[0;37m";
    inline constexpr FColor FColor::White =         "\033[1;37m";


}

#endif // ENGINE_CORE_UTILITIES_COLOR_H
