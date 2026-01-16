#ifndef ENGINE_CORE_UTILITIES_DEBUG_H
#define ENGINE_CORE_UTILITIES_DEBUG_H

#include "Time.h"
#include "FColor.h"
#include <Windows.h>
#include <iostream>
#include <string_view>
#include <format>
#include <sstream>

enum class LogCategory{ 
	Engine,		// General startup/shutdown
	Editor,		// UI and Tooling logic
	Physics,	// Collision and Rigidbody updates
	Rendering,	// High-level: Mesh, Lights, Camera
	OpenGL,		// Low-level: Buffers, Gl-Errors
	IO,			// File loading (PathManager)
	Temp,		// For temporarily debugging
	Material,	// Shaders, Texture, Uniforms
};

class Debug
{
public:

	// Basic Logs (White)
	template<typename... Args>
	static void Log(LogCategory category, std::string_view message, Args&&... args) {
		Print(category, "LOG", FColor::White, message, std::forward<Args>(args)...);
	}

	template<typename... Args>
	static void LogWarning(LogCategory category, std::string_view message, Args&&... args) {
		Print(category, "WARNING", FColor::Yellow, message, std::forward<Args>(args)...);
	}

	template<typename... Args>
	static void LogError(LogCategory category, std::string_view message, Args&&... args) {
		Print(category, "ERROR", FColor::Red, message, std::forward<Args>(args)...);
	}

private:
	template<typename... Args>
	static void Print(LogCategory category, const char* level, FColor color, std::string_view message, Args&&... args) {

		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

		// Get time stamp
		std::string formattedMessage = FormatMessage(message, std::forward<Args>(args)...);

		// Set color based on log level and print prefix [Time][Level][Category]
		SetConsoleTextAttribute(hConsole, FColor::DarkGrey); // Dark grey for timestamp
		std::cout << '[' << Time::CreateTimeStamp() << "]: ";

		SetConsoleTextAttribute(hConsole, color); // Color for log level
		std::cout << '[' << level << "][" << CategoryToString(category) << "] " << formattedMessage << std::endl;
	}

	static std::string_view CategoryToString(LogCategory category) {
		switch(category) {
			case LogCategory::Engine:		return "Engine";
			case LogCategory::Editor:		return "Editor";
			case LogCategory::Physics:		return "Physics";
			case LogCategory::Rendering:	return "Rendering";
			case LogCategory::OpenGL:		return "OpenGL";
			case LogCategory::IO:			return "IO";
			case LogCategory::Temp:			return "Temp";
			case LogCategory::Material:		return "Material";
			default:						return "General";
		}
	}

	template<typename... Args>
	static std::string FormatMessage(std::string_view fmt, Args&&... args) {

		// If no args, just return the string
		if constexpr (sizeof...(args) == 0) return std::string(fmt);

		// Use stringstream to handle types that have operator<<
		std::stringstream ss;
		size_t argIndex = 0;
		auto argTuple = std::make_tuple(std::forward<Args>(args)...);

		// This is a simplified manual parser for the ({}) brackets
		for (size_t i = 0; i < fmt.length(); ++i) {
			if (fmt[i] == '{' && i + 1 < fmt.length() && fmt[i + 1] == '}') {

				// Insert the next argument from our tuple into the stream
				std::apply([&ss, argIndex](auto&&... unpackedArgs){
					size_t currentIndex = 0;
					((currentIndex++ == argIndex ? (ss << unpackedArgs, 0) : 0), ...);
				}, argTuple);

				argIndex++;
				i++; // Skip the '}'
			}
			else {
				ss << fmt[i];
			}
		}
		return ss.str();
	}
};

#pragma region SHARK ENGINE LOGGING MACROS

// Standard Log (White/BrightWhite)
#define SHARK_LOG(Category, Message, ...) \
	Debug::Log(LogCategory::Category, Message, ##__VA_ARGS__)

// Warning (Yellow) - Pointing to LogWarning
#define SHARK_WARN(Category, Message, ...) \
	Debug::LogWarning(LogCategory::Category, Message, ##__VA_ARGS__)

// Error (Red) - Pointing to LogError
#define SHARK_ERR(Category, Message, ...) \
	Debug::LogError(LogCategory::Category, Message, ##__VA_ARGS__)
#pragma endregion

#endif // ENGINE_CORE_UTILITIES_DEBUG_H