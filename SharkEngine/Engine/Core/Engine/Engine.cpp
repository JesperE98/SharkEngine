// Engine.cpp : Defines the functions for the static library.
//
#include "Engine.h"
#include "EngineContext.h"
#include "Graphics/Rendering/Renderer.h"
#include "pch.h"
#include "framework.h"

#include <iostream>

namespace Shark::Core {

	using Shark::Graphics::Renderer;

	// TODO: This is an example of a library function
	void fnEngine()
	{
	}

	Engine::Engine()
	{
		SHARK_LOG(Engine, "Engine::Engine() - Engine created.");
	}

	Engine::~Engine()
	{
		SHARK_LOG(Engine, "Engine::~Engine() - Engine destroyed.");
	}

	void Engine::Initialize()
	{
		SHARK_LOG(Engine, "Engine::Initialize() - Initializing Engine.");

		if (!glfwInit())
		{
			SHARK_ERR(Engine, "Engine::Initialize() - Failed to initialize GLFW.");
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
			SHARK_ERR(Engine, "Engine::Initialize() - Failed to create GLFW window.");
			glfwTerminate();
			return;
		}

		glfwMakeContextCurrent(EngineContext::Get().m_Window);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << Time::CreateTimeStamp() << ": [Engine] Failed to initialize GLAD" << std::endl;
			SHARK_ERR(Engine, "Engine::Initialize() - Failed to initialize GLAD.");
			return;
		}

		glfwSetFramebufferSizeCallback(EngineContext::Get().m_Window, Renderer::FramebufferSizeCallback);

		SHARK_LOG(Engine, "Engine::Initialize() - Engine setup complete. Entering main loop.");
	}

	void Engine::Run()
	{
		if (!EngineContext::Get().m_Window)
		{
			SHARK_ERR(Engine, "Engine::Run() - Cannot run engine without a valid window.");
			return;
		}
	}

	void Engine::Shutdown()
	{
		SHARK_LOG(Engine, "Engine::Shutdown() - Shutting down engine.");

		if (EngineContext::Get().m_Window) {
			glfwDestroyWindow(EngineContext::Get().m_Window);
			glfwTerminate();
		}

		SHARK_LOG(Engine, "Engine::Shutdown() - Engine shutdown complete.");
	}

	bool Engine::IsWindowClosed()
	{
		if (!EngineContext::Get().m_Window) {
			return false;
		}

		return glfwWindowShouldClose(EngineContext::Get().m_Window);
	}

}