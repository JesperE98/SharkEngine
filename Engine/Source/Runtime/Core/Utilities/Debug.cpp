#include "Debug.h"
#include "Core/Events/SharkEvents.h"

#include <iostream>
#include <glad/glad.h>

namespace Shark::Core {

	// Helper to strip full path down to just the filename
	static const char* GetShortFileName(const char* path) {
		if (!path) return "";
		
		const char* file = std::strrchr(path, '\\');
		if (!file) file = std::strrchr(path, '/');

		return file ? file + 1 : path;
	}

	// Helper to format the argument substitution - shared between overloads
	static std::string FormatMessage(std::string_view message, const std::vector<std::string>& args) {
		std::stringstream ss;
		size_t argIndex = 0;

		for (size_t i = 0; i < message.length(); ++i) {
			if (message[i] == '{' && i + 1 < message.length() && message[i + 1] == '}') {
				if (argIndex < args.size()) {
					ss << args[argIndex++];
				}
				i++;
			}
			else {
				ss << message[i];
			}
		}

		return ss.str();
	}

	// --- Info-level logs (Log, Success, Request, Process) ---
	void Debug::LogInternal(LogCategory category, const char* level, FColor color, const char* function, std::string_view message, const std::vector<std::string>& args)
	{
		// Use stringstream to handle types that have operator<<
		std::string timeStamp = Time::CreateTimeStamp();

		// Prepend function name
		std::string fullMessage = std::string(function) + "() - " + FormatMessage(message, args);

		LogEntry entry{ timeStamp, level, category, fullMessage };

		if (m_LogHistory.size() >= MAX_LOG_HISTORY) {
			m_LogHistory.erase(m_LogHistory.begin(), m_LogHistory.begin() + (MAX_LOG_HISTORY / 10));
		}
		m_LogHistory.push_back(entry);

		SharkEvents::OnLogAdded().Broadcast(entry);

		std::cout << 
			FColor::DarkGrey.Code << '[' << timeStamp << "]: "
			<< color.Code << '[' << level << "] "
			<< CategoryToString(category) << ": "
			<< fullMessage << FColor::White.Code
			<< std::endl;
	}

	// --- Error-level logs (Warning, Error, Fatal) with file + line ---
	void Debug::LogInternal(LogCategory category, const char* level, FColor color, const char* file, int line, const char* function, std::string_view message, const std::vector<std::string>& args)
	{
		std::string timeStamp			= Time::CreateTimeStamp();

		// Build full message: "ShortFile.cpp:42 OnInitialize() - message"
		std::stringstream prefix;
		prefix << GetShortFileName(file) << ":" << line << " " << function << "() - ";
		std::string fullMessage = prefix.str() + FormatMessage(message, args);

		LogEntry entry{ timeStamp, level, category, fullMessage };

		if (m_LogHistory.size() >= MAX_LOG_HISTORY) {
			m_LogHistory.erase(m_LogHistory.begin(), m_LogHistory.begin() + (MAX_LOG_HISTORY / 10));
		}
		m_LogHistory.push_back(entry);

		SharkEvents::OnLogAdded().Broadcast(entry);

		std::cout <<
			FColor::DarkGrey.Code << '[' << timeStamp << "]: "
			<< color.Code << '[' << level << "] "
			<< CategoryToString(category) << ": "
			<< fullMessage << FColor::White.Code
			<< std::endl;
	}

	void Debug::CheckGLErrors(const std::string& context)
	{
		GLenum err;
		while ((err = glGetError()) != GL_NO_ERROR) {
			std::string errorStr;
			switch (err) {
			case GL_INVALID_ENUM:					errorStr = "INVALID_ENUM"; break;
			case GL_INVALID_VALUE:					errorStr = "INVALID_VALUE"; break;
			case GL_INVALID_OPERATION:				errorStr = "INVALID_OPERATION"; break;
			case GL_OUT_OF_MEMORY:					errorStr = "OUT_OF_MEMORY"; break;
			case GL_INVALID_FRAMEBUFFER_OPERATION:	errorStr = "INVALID_FRAMEBUFFER_OPERATION"; break;
			default:								errorStr = "UNKNOWN_ERROR"; break;
			}
			SE_ERR(OpenGL, "GL Error at {}: {} (Code: {}", context, errorStr, err);
		}
	}

	std::string_view Debug::CategoryToString(LogCategory category) {
		switch (category) {
		case LogCategory::Engine:		return "Engine";
		case LogCategory::Editor:		return "Editor";
		case LogCategory::Physics:		return "Physics";
		case LogCategory::Rendering:	return "Rendering";
		case LogCategory::OpenGL:		return "OpenGL";
		case LogCategory::IO:			return "IO";
		case LogCategory::Temp:			return "Temp";
		case LogCategory::Resources:	return "Resources";
		case LogCategory::Messaging:	return "Messaging";
		default:						return "General";
		}
	}
}