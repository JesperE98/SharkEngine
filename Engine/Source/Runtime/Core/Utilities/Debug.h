#ifndef ENGINE_CORE_UTILITIES_DEBUG_H
#define ENGINE_CORE_UTILITIES_DEBUG_H

#include "Time.h"
#include "FColor.h"
#include <string>
#include <string_view>
#include <vector>
#include <sstream>

namespace Shark::Core {

	enum class LogCategory : uint8_t {
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

	struct LogEntry {
		std::string TimeStamp;
		std::string Level;
		LogCategory Category;
		std::string Message;
	};

	class Debug
	{
	public:

		// Basic Logs (White)
		template<typename... Args>
		static void Log(LogCategory category, const char* function, std::string_view message, Args&&... args) {
			LogInternal(category, "LOG", FColor::White, function, message, PackArgs(std::forward<Args>(args)...));
		}

		template<typename... Args>
		static void LogWarning(LogCategory category, const char* file, int line, const char* function, std::string_view message, Args&&... args) {
			LogInternal(category, "WARNING", FColor::Yellow, file, line, function, message, PackArgs(std::forward<Args>(args)...));
		}

		template<typename... Args>
		static void LogError(LogCategory category, const char* file, int line, const char* function, std::string_view message, Args&&... args) {
			LogInternal(category, "ERROR", FColor::Red, file, line, function, message, PackArgs(std::forward<Args>(args)...));
		}

		template<typename... Args>
		static void LogSuccess(LogCategory category, const char* function, std::string_view message, Args&&... args) {
			LogInternal(category, "SUCCESS", FColor::Green, function, message, PackArgs(std::forward<Args>(args)...));
		}

		template<typename... Args>
		static void LogRequest(LogCategory category, const char* function, std::string_view message, Args&&... args) {
			LogInternal(category, "REQUEST", FColor::Cyan, function, message, PackArgs(std::forward<Args>(args)...));
		}

		template<typename... Args>
		static void LogProcess(LogCategory category, const char* function, std::string_view message, Args&&... args) {
			LogInternal(category, "PROCESS", FColor::Cyan, function, message, PackArgs(std::forward<Args>(args)...));
		}

		template<typename... Args>
		static void LogFatal(LogCategory category, const char* file, int line, const char* function, std::string_view message, Args&&... args) {
			LogInternal(category, "FATAL", FColor::DarkRed, file, line, function, message, PackArgs(std::forward<Args>(args)...));

			throw std::runtime_error("Engine Fatal Error: Check Console");
		}

		// List of all the logs
		static const std::vector<LogEntry> GetLogHistory() { return m_LogHistory; }
		static void ClearLogHistory() { m_LogHistory.clear(); }
		static std::string_view CategoryToString(LogCategory category);		
		
		/*
		@brief Utility function to check for OpenGL errors after rendering operations. This function queries the OpenGL error state and logs any errors that are found,
		along with a provided context string to help identify where the error occurred in the code.
		@param context A string describing the context in which the OpenGL error check is being performed (e.g., "After ImGui Initialization").
		*/
		static void CheckGLErrors(const std::string& context);

	private:

		static inline std::vector<LogEntry> m_LogHistory;
		static constexpr size_t MAX_LOG_HISTORY = 10000;

		static void LogInternal(
			LogCategory category,
			const char* level,
			FColor color,
			const char* function,
			std::string_view message,
			const std::vector<std::string>& args);

		static void LogInternal(
			LogCategory category,
			const char* level,
			FColor color,
			const char* file,
			int line,
			const char* function,
			std::string_view message,
			const std::vector<std::string>& args);

		template<typename... Args>
		static std::vector<std::string> PackArgs(Args&&... args) {
			std::vector<std::string> result;

			([&](auto&& arg) {
				std::stringstream ss;
				ss << arg;
				result.push_back(ss.str());
				}(std::forward<Args>(args)), ...);

			return result;
		}
	};
}

#pragma region SHARK ENGINE LOGGING MACROS

// Standard Log (White/BrightWhite)
#define SE_LOG(Category, Message, ...) \
	Shark::Core::Debug::Log(Shark::Core::LogCategory::Category, __func__, Message, ##__VA_ARGS__)

// Warning (Yellow) - Pointing to LogWarning
#define SE_WARN(Category, Message, ...) \
	Shark::Core::Debug::LogWarning(Shark::Core::LogCategory::Category, \
	__FILE__, __LINE__, __func__, Message, ##__VA_ARGS__)

// Error (Red) - Pointing to LogError
#define SE_ERR(Category, Message, ...) \
	Shark::Core::Debug::LogError(Shark::Core::LogCategory::Category, \
	__FILE__, __LINE__, __func__, Message, ##__VA_ARGS__)

// Success (Green) - Pointing to LogSuccess
#define SE_SUCC(Category, Message, ...) \
	Shark::Core::Debug::LogSuccess(Shark::Core::LogCategory::Category, __func__, Message, ##__VA_ARGS__)

// Request (Cyan) - Pointing to LogRequest
#define SE_REQ(Category, Message, ...) \
	Shark::Core::Debug::LogRequest(Shark::Core::LogCategory::Category, __func__, Message, ##__VA_ARGS__)

// Process (Cyan) - Pointing to LogProcess
#define SE_PROC(Category, Message, ...) \
	Shark::Core::Debug::LogProcess(Shark::Core::LogCategory::Category, __func__, Message, ##__VA_ARGS__)

// Fatal Error (DarkRed) - Pointing to LogFatal
#define SE_FAT(Category, Message, ...) \
	Shark::Core::Debug::LogFatal(Shark::Core::LogCategory::Category, \
	__FILE__, __LINE__, __func__, Message, ##__VA_ARGS__)
#pragma endregion

#endif // ENGINE_CORE_UTILITIES_DEBUG_H