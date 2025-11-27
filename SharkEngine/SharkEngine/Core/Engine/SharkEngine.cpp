#include "SharkEngine.h"
#include "EngineContext.h"
#include "Source/Graphics/Rendering/Renderer.h"
#include "Core/Utilities/Time.h"

#include <iostream>


SharkEngine::SharkEngine()
{
	std::cout << Time::CreateTimeStamp() << ": " << "Engine created." << std::endl;
}

SharkEngine::~SharkEngine()
{
	std::cout << Time::CreateTimeStamp() << ": " << "Engine destroyed." << std::endl;
}

void SharkEngine::Initialize()
{
	std::cout << Time::CreateTimeStamp() << ": " << "Engine initializizing..." << std::endl;

	if(!glfwInit())
	{
		std::cout << Time::CreateTimeStamp() << ": " << "Failed to initialize GLFW." << std::endl;
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

	if(!EngineContext::Get().m_Window)
	{
		std::cout << Time::CreateTimeStamp() << ": " << "Failed to create GLFW window." << std::endl;
		glfwTerminate();
		return;
	}

	glfwMakeContextCurrent(EngineContext::Get().m_Window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << Time::CreateTimeStamp() << ": " << "Failed to initialize GLAD" << std::endl;
		return;
	}

	glfwSetFramebufferSizeCallback(EngineContext::Get().m_Window, Renderer::FramebufferSizeCallback);

	std::cout << Time::CreateTimeStamp() << ": " << "Engine setup complete. Entering main loop." << std::endl;
}

void SharkEngine::Run()
{
	if(!EngineContext::Get().m_Window)
	{
		std::cout << Time::CreateTimeStamp() << ": " << "Cannot run engine without a valid window." << std::endl;
		return;
	}
}

void SharkEngine::Shutdown()
{
	std::cout << Time::CreateTimeStamp() << ": " << "Engine shutting down..." << std::endl;

	if (EngineContext::Get().m_Window) {
		glfwDestroyWindow(EngineContext::Get().m_Window);
		glfwTerminate();
	}

	std::cout << Time::CreateTimeStamp() << ": " << "Engine shutdown complete." << std::endl;
}

bool SharkEngine::IsWindowClosed()
{
	if (!EngineContext::Get().m_Window) {
		return false;
	}

	return glfwWindowShouldClose(EngineContext::Get().m_Window);
}
