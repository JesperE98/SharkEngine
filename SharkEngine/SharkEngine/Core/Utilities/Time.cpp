#include "Time.h"

#include <chrono>
#include <GLFW/glfw3.h>

double Time::m_LastFrame;
double Time::m_CurrentFrame;
double Time::m_DeltaTime;

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

double Time::GetLastFrame()
{
	return m_LastFrame = glfwGetTime();
}

double Time::GetCurrentFrame()
{
	return m_CurrentFrame = glfwGetTime();
}

double Time::GetDeltaTime()
{
	m_DeltaTime = m_CurrentFrame - m_LastFrame;
	m_LastFrame = m_CurrentFrame;
	return m_DeltaTime;
}
