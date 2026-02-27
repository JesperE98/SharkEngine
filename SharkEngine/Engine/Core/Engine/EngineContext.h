#ifndef ENGINE_CONTEXT_H
#define ENGINE_CONTEXT_H

#include "Engine.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Shark::Graphics	{ class Renderer; }

namespace Shark::Core {

	class EngineContext
	{
	public:
#pragma region Core Engine objects
		GLFWwindow* m_Window{ nullptr };
		Engine* m_Engine{ nullptr };
		Shark::Graphics::Renderer* m_Renderer{ nullptr };
#pragma endregion
		// Singleton accessor
		static EngineContext& Get() {
			static EngineContext instance;
			return instance;
		}



		// Initilizes engine, renderer, scene
		void OnInitialize();
		// Update engine + scene logic
		void OnUpdate(float deltaTime);
		// Shutdown and cleanup
		void OnEnd();

	private:
		EngineContext() = default;
		~EngineContext() = default;

		EngineContext(const EngineContext&) = delete;
		EngineContext& operator = (const EngineContext&) = delete;

	};
}


#endif