#ifndef ENGINE_CONTEXT_H
#define ENGINE_CONTEXT_H

#include <glad/glad.h>

struct GLFWwindow;

namespace Shark::Graphics	{ class Renderer; }

namespace Shark::Core {

	/**
	 * @brief Singleton that manages global engine state and subsystems (window and renderer) and provides lifecycle methods.
	 */
	class EngineContext
	{
	public:
		// Singleton accessor
		static EngineContext& Get() {
			static EngineContext instance;
			return instance;
		}

		void Initialize();
		void Update(float deltaTime);
		void End();

		void SetWindow(GLFWwindow* window) { m_Window = window; }
		GLFWwindow* GetWindow() const { return m_Window; }

		Graphics::Renderer* GetRenderer() const { return m_Renderer; }

		EngineContext(const EngineContext&)				= delete;
		EngineContext& operator=(const EngineContext&)	= delete;

	private:
		GLFWwindow* m_Window			= nullptr;
		Graphics::Renderer* m_Renderer	= nullptr;

		EngineContext() = default;
		~EngineContext() = default;
	};
}


#endif