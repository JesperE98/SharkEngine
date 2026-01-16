#ifndef TIME_H
#define TIME_H

#include <string>

class Time
{
public:
	/* Returns the current time formatted as "YYYY-MM-DD HH:MM:SS" */
	static std::string CreateTimeStamp();

	static double GetLastFrame();
	// Returns the current time
	static double GetCurrentFrame();
	static double GetDeltaTime();

private:
#pragma region Member Variables
	static double m_LastFrame;
	static double m_CurrentFrame;
	static double m_DeltaTime;
#pragma endregion
};

#endif