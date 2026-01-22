#ifndef ENGINE_H
#define ENGINE_H

#include "Core/Utilities/Debug.h"
#include <memory>

namespace Shark::Core{

#pragma region Window Settings
	static constexpr unsigned int WINDOW_WIDTH = 1280;
	static constexpr unsigned int WINDOW_HEIGHT = 840;
	const std::string WINDOW_TITLE = "SharkEngine Window";
#pragma endregion

	class Engine
	{
	public:
		Engine();
		~Engine();

		void Initialize();
		void Run();
		void Shutdown();

		bool IsWindowClosed();

	private:
#pragma region Engine Member Variables
		double m_LastFrame = 0.0f;
		double m_CurrentFrame = 0.0f;
		double m_DeltaTime = 0.0f;
#pragma endregion


	};
}


#endif