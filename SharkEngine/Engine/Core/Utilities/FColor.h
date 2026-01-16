#ifndef ENGINE_CORE_UTILITIES_COLOR_H
#define ENGINE_CORE_UTILITIES_COLOR_H

#include <Windows.h>

// FColor struct to represent Windows Console Color Codes
struct FColor {

	WORD Code;

	// Constructor to allow "static constexpr FColor Red = 12"
	constexpr FColor(WORD InCode) : Code(InCode) {}

	// Implicit conversion to WORD
	operator WORD() const { return Code; }


	// Windows Console Color Codes
	static const FColor Black;
	static const FColor DarkBlue;
	static const FColor DarkGreen;
	static const FColor DarkCyan;
	static const FColor DarkRed;
	static const FColor DarkMagenta;
	static const FColor DarkYellow;
	static const FColor LightGrey;
	static const FColor DarkGrey;
	static const FColor Blue;
	static const FColor Green;
	static const FColor Cyan;
	static const FColor Red;
	static const FColor Magenta;
	static const FColor Yellow;
	static const FColor White;
};

inline constexpr FColor FColor::Black		= 0;
inline constexpr FColor FColor::DarkBlue	= 1;
inline constexpr FColor FColor::DarkGreen	= 2;
inline constexpr FColor FColor::DarkCyan	= 3;
inline constexpr FColor FColor::DarkRed		= 4;
inline constexpr FColor FColor::DarkMagenta = 5;
inline constexpr FColor FColor::DarkYellow	= 6;
inline constexpr FColor FColor::LightGrey	= 7;
inline constexpr FColor FColor::DarkGrey	= 8;
inline constexpr FColor FColor::Blue		= 9;
inline constexpr FColor FColor::Green		= 10;
inline constexpr FColor FColor::Cyan		= 11;
inline constexpr FColor FColor::Red			= 12;
inline constexpr FColor FColor::Magenta		= 13;
inline constexpr FColor FColor::Yellow		= 14;
inline constexpr FColor FColor::White		= 15;


#endif // ENGINE_CORE_UTILITIES_COLOR_H
