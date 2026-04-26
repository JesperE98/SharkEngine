#include "Engine.h"
#include "EngineContext.h"
#include "Core/App/Application.h"
#include "Core/Utilities/Time.h"
#include "Graphics/Rendering/Renderer.h"
#include "pch.h"
#include "framework.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Shark::Core {

	using Graphics::Renderer;

	// TODO: This is an example of a library function
	void fnEngine()
	{
	}

	Engine::Engine() { SE_LOG(Engine, "Engine created."); }
	Engine::~Engine() { SE_LOG(Engine, "Engine destroyed."); }

	void Engine::Initialize() {
		SE_PROC(Engine, "Initializing Engine...");

		if (!glfwInit())
		{
			SE_ERR(Engine, "Failed to initialize GLFW.");
			return;
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

		/* ----------------- Create Window ----------------- */
		m_Window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE.c_str(), nullptr, nullptr);

		if (!m_Window) {
			SE_ERR(Engine, "Failed to create GLFW window.");
			glfwTerminate();
			return;
		}

		glfwMakeContextCurrent(m_Window);
		glfwSwapInterval(1); // enables VSync, caps to monitor refresh rate

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			SE_ERR(Engine, "Failed to initialize GLAD.");
			return;
		}

		glfwSetFramebufferSizeCallback(m_Window, Renderer::FramebufferSizeCallback);

		EngineContext::Get().SetWindow(m_Window);

		m_Initialized = true;
		SE_SUCC(Engine, "Engine setup complete.");
	}

	void Engine::Run(Application* app)
	{
		if (!m_Initialized) {
			SE_ERR(Engine, "Must call Initialize() first.");
			return;
		}

		// Init engine subsystems + application
		EngineContext::Get().Initialize();
		if (app) app->OnInitialize();

		// Main loop
		while (!glfwWindowShouldClose(m_Window)) {
			glfwPollEvents();

			Time::Update();
			float dt = Time::GetDeltaTime();

			EngineContext::Get().Update(dt);

			// Engine-side update (editor UI frame, game logic hooks)
			if (app) {
				app->OnUpdate(dt);
				app->OnRender();
			}

			glfwSwapBuffers(m_Window);
		}

		// Shutdown - reverse order
		if (app) app->OnShutdown();
		EngineContext::Get().End();
	}

	void Engine::Shutdown()
	{
		SE_PROC(Engine, "Shutting down engine...");

		if (m_Window) {
			glfwDestroyWindow(m_Window);
			glfwTerminate();
			m_Window = nullptr;
		}

		SE_SUCC(Engine, "Engine shutdown complete.");
	}
}