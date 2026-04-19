#ifndef TIME_H
#define TIME_H

#include <string>

namespace Shark::Core {
	class Time
	{
	public:
		/* Returns the current time formatted as "YYYY-MM-DD HH:MM:SS" */
		static std::string CreateTimeStamp();

		static void Update();
		static float GetDeltaTime() { return static_cast<float>(m_DeltaTime); }
		static double GetCurrentFrame() { return m_CurrentFrame; }

	private:
#pragma region Member Variables
		static double m_LastFrame;
		static double m_CurrentFrame;
		static double m_DeltaTime;
#pragma endregion
	};

}

#endif