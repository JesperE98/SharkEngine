#include "Engine.h"
#include "EngineContext.h"
#include "Graphics/Rendering/Renderer.h"
#include "pch.h"
#include "framework.h"

#include <glad.h>

namespace Shark::Core {

	using Shark::Graphics::Renderer;

	// TODO: This is an example of a library function
	void fnEngine()
	{
	}

	Engine::Engine()
	{
		SE_LOG(Engine, "Engine::Engine() - Engine created.");
	}

	Engine::~Engine()
	{
		SE_LOG(Engine, "Engine::~Engine() - Engine destroyed.");
	}

	void Engine::OnInitialize()
	{
		SE_LOG(Engine, "Engine::OnInitialize() - Initializing Engine.");

		if (!glfwInit())
		{
			SE_ERR(Engine, "Engine::OnInitialize() - Failed to initialize GLFW.");
			return;
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);



#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

		/* ----------------- Create Window ----------------- */
		EngineContext::Get().m_Window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE.c_str(), nullptr, nullptr);

		if (!EngineContext::Get().m_Window)
		{
			SE_ERR(Engine, "Engine::OnInitialize() - Failed to create GLFW window.");
			glfwTerminate();
			return;
		}

		glfwMakeContextCurrent(EngineContext::Get().m_Window);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			SE_ERR(Engine, "Engine::OnInitialize() - Failed to initialize GLAD.");
			return;
		}

		glfwSetFramebufferSizeCallback(EngineContext::Get().m_Window, Renderer::FramebufferSizeCallback);

		SE_LOG(Engine, "Engine::OnInitialize() - Engine setup complete. Entering main loop.");
	}

	void Engine::Run()
	{
		if (!EngineContext::Get().m_Window)
		{
			SE_ERR(Engine, "Engine::Run() - Cannot run engine without a valid window.");
			return;
		}
	}

	void Engine::Shutdown()
	{
		SE_LOG(Engine, "Engine::Shutdown() - Shutting down engine.");

		if (EngineContext::Get().m_Window) {
			glfwDestroyWindow(EngineContext::Get().m_Window);
			glfwTerminate();
		}

		SE_LOG(Engine, "Engine::Shutdown() - Engine shutdown complete.");
	}

	bool Engine::IsWindowClosed()
	{
		if (!EngineContext::Get().m_Window) {
			return false;
		}

		return glfwWindowShouldClose(EngineContext::Get().m_Window);
	}

}