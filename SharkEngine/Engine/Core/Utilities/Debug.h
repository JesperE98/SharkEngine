#ifndef ENGINE_CORE_UTILITIES_DEBUG_H
#define ENGINE_CORE_UTILITIES_DEBUG_H

#include "Time.h"
#include "FColor.h"
#include <iostream>
#include <string_view>
#include <format>
#include <sstream>

namespace Shark::Core {

	enum class LogCategory {
		Engine,		// General startup/shutdown
		Editor,		// UI and Tooling logic
		Physics,	// Collision and Rigidbody updates
		Rendering,	// High-level: Mesh, Lights, Camera
		OpenGL,		// Low-level: Buffers, Gl-Errors
		IO,			// File loading (PathManager)
		Temp,		// For temporarily debugging
		Resources,	// Shaders, Texture, Uniforms
		Messaging,	// Message Queue related logs
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

		template<typename... Args>
		static void LogSuccess(LogCategory category, std::string_view message, Args&&... args) {
			Print(category, "SUCCESS", FColor::Green, message, std::forward<Args>(args)...);
		}

		template<typename... Args>
		static void LogRequest(LogCategory category, std::string_view message, Args&&... args) {
			Print(category, "REQUEST", FColor::Cyan, message, std::forward<Args>(args)...);
		}

		template<typename... Args>
		static void LogProcess(LogCategory category, std::string_view message, Args&&... args) {
			Print(category, "PROCESS", FColor::Cyan, message, std::forward<Args>(args)...);
		}

		template<typename... Args>
		static void LogFatal(LogCategory category, std::string_view message, Args&&... args) {

            // With the following two lines:
            std::string fatalMsg = FormatMessage(message, std::forward<Args>(args)...);
            Print(category, "FATAL", FColor::DarkRed, fatalMsg);
            throw std::runtime_error(fatalMsg);
		}

	private:

		static std::string_view CategoryToString(LogCategory category);

		template<typename... Args>
		static void Print(LogCategory category, const char* level, FColor color, std::string_view message, Args&&... args) {

			// Get time stamp
			std::string formattedMessage = FormatMessage(message, std::forward<Args>(args)...);

			// Set color based on log level and print prefix [Time][Level][Category]
			std::cout << FColor::DarkGrey.Code <<'[' << Time::CreateTimeStamp() << "]: ";

			std::cout << color.Code << '[' << CategoryToString(category) << "]	[" << level << "] " << formattedMessage << FColor::White.Code << std::endl;
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
					std::apply([&ss, argIndex](auto&&... unpackedArgs) {
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
}

#pragma region SHARK ENGINE LOGGING MACROS

// Standard Log (White/BrightWhite)
#define SE_LOG(Category, Message, ...) \
	Shark::Core::Debug::Log(Shark::Core::LogCategory::Category, Message, ##__VA_ARGS__)

// Warning (Yellow) - Pointing to LogWarning
#define SE_WARN(Category, Message, ...) \
	Shark::Core::Debug::LogWarning(Shark::Core::LogCategory::Category, Message, ##__VA_ARGS__)

// Error (Red) - Pointing to LogError
#define SE_ERR(Category, Message, ...) \
	Shark::Core::Debug::LogError(Shark::Core::LogCategory::Category, Message, ##__VA_ARGS__)

// Success (Green) - Pointing to LogSuccess
#define SE_SUCC(Category, Message, ...) \
	Shark::Core::Debug::LogSuccess(Shark::Core::LogCategory::Category, Message, ##__VA_ARGS__)

// Request (Cyan) - Pointing to LogRequest
#define SE_REQ(Category, Message, ...) \
	Shark::Core::Debug::LogRequest(Shark::Core::LogCategory::Category, Message, ##__VA_ARGS__)

// Process (Cyan) - Pointing to LogProcess
#define SE_PROC(Category, Message, ...) \
	Shark::Core::Debug::LogProcess(Shark::Core::LogCategory::Category, Message, ##__VA_ARGS__)

// Fatal Error (DarkRed) - Pointing to LogFatal
#define SE_FAT(Category, Message, ...) \
	Shark::Core::Debug::LogFatal(Shark::Core::LogCategory::Category, Message, ##__VA_ARGS__)
#pragma endregion

#endif // ENGINE_CORE_UTILITIES_DEBUG_H