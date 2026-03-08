#include "Time.h"

#include <chrono>
#include <GLFW/glfw3.h>

namespace Shark::Core {
	double Time::m_LastFrame = 0.0f;
	double Time::m_CurrentFrame = 0.0f;
	double Time::m_DeltaTime = 0.0f;

	std::string Time::CreateTimeStamp()
	{
		// Creates static varaiables that retain their values between function calls
		static std::string lastTime; // Stores the last formatted timestamp
		static std::time_t lastSec = 0; // Stores the last second value checked

		// Get current time as a time_t (seconds since epoch)
		const auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

		// If the current second is different from the last checked second, update the timestamp
		if (now != lastSec)
		{
			lastSec = now; // Update last checked second
			std::tm tm_buf; // Temporary struct to hold	broken-down time
			localtime_s(&tm_buf, &now); // Convert time_t to local time structure (thread-safe version)

			char buffer[20];
			// Format the time into a string: YYYY-MM-DD HH:MM:SS
			std::strftime(buffer, sizeof(buffer), "%H:%M:%S", &tm_buf);

			lastTime = buffer; // Store the formatted timestamp
		}

		// Return the last formatted timestamp
		return lastTime;
	}

	void Time::Update() {
		m_CurrentFrame = glfwGetTime();
		m_DeltaTime = m_CurrentFrame - m_LastFrame;
		m_LastFrame = m_CurrentFrame;
	}
}
