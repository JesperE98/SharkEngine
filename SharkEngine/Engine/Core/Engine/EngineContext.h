#ifndef ENGINE_CONTEXT_H
#define ENGINE_CONTEXT_H

#include "Engine.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Shark::Graphics	{ class Renderer; }
namespace Shark				{ class Scene; }

namespace Shark::Core {

	class EngineContext
	{
	public:
		// Singleton accessor
		static EngineContext& Get() {
			static EngineContext instance;
			return instance;
		}

#pragma region Core Engine objects
		Engine* m_Engine{ nullptr };
		Shark::Scene* m_Scene{ nullptr };
		Shark::Graphics::Renderer* m_Renderer{ nullptr };
		GLFWwindow* m_Window{ nullptr };
#pragma endregion

		// Initilizes engine, renderer, scene
		void OnInit();
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