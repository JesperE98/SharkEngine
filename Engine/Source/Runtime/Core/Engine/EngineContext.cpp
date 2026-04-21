/* Plan (pseudocode):
   1. Problem: GL_DEBUG_OUTPUT is undefined at compile time.
   2. Root cause: Platform GL headers provided by GLFW may be included before glad, causing missing GL symbols
      (glad must be included before GLFW so glad supplies the OpenGL declarations/macros).
   3. Fix: Ensure <glad/glad.h> is included before <GLFW/glfw3.h> in this translation unit.
   4. Minimal change: swap the two include lines; keep all other code unchanged.
   5. Result: GL_DEBUG_OUTPUT and related debug enums will be available and the compile error will be resolved.
*/


#include "EngineContext.h"
#include "Engine.h"
#include "Graphics/Rendering/ForwardRenderer.h"
#include "Graphics/Resources/MeshManager.h"
#include "Graphics/Resources/PrimitiveMesh.h"
#include "Graphics/Resources/ShaderManager.h"
#include "Graphics/Resources/TextureManager.h"
#include "Input/InputManager.h"
#include "IO/Pathmanager.h"
#include "Memory/MemoryManager.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"


namespace Shark::Core
{
#ifndef GL_DEBUG_OUTPUT
#define GL_DEBUG_OUTPUT 0x92E0
#define GL_DEBUG_OUTPUT_SYNCHRONOUS 0x8242
#define GL_DEBUG_TYPE_ERROR 0x8249
#define GL_DEBUG_SEVERITY_NOTIFICATION 0x826B
#define GL_DEBUG_SEVERITY_HIGH 0x9146
#define GL_DEBUG_SEVERITY_MEDIUM 0x9147
#define GL_DEBUG_SEVERITY_LOW 0x9148
#endif
	// The function pointer type for the callback
	typedef void (APIENTRY* GLDEBUGPROC)(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);

	using IO::PathManager;
	using Input::InputManager;
	using Memory::MemoryManager;
	using Resources::MeshManager;
	using Resources::ShaderManager;
	using Resources::TextureManager;
	using Graphics::ForwardRenderer;
	using Graphics::PrimitiveType;

	void APIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
		GLsizei length, const GLchar* message, const void* userParam)
	{
		// Filter out the "Notification" severity to avoid spamming the log
		if (severity == 0x826B) return;

		std::string level = (type == 0x8249) ? " [ERROR] " : " [DEBUG] ";

		// This will print the EXACT reason OpenGL is unhappy
		std::cout << "OpenGL" << level << "ID: " << id << " | Message: " << message << std::endl;

		// You can use your SE_ERR macro here too
		// SE_ERR(OpenGL, "ID: {} | Message: {}", id, message);
	}

	void EngineContext::PreInitialize()
	{
		/* ----------------- OnInitialize Engine ----------------- */
		m_Engine = new Engine();
		m_Engine->OnInitialize();
	}

	void EngineContext::OnInitialize() {
		SE_LOG(Engine, "EngineContext::OnInitialize() - Initializing EngineContext!");

		/* ----------------- OnInitialize Managers ----------------- */
		InputManager::Get().OnInitialize(m_Window);
		PathManager::Get().OnInitialize();

		/* ----------------- Create Renderer ----------------- */
		m_Renderer = new ForwardRenderer();

		/* ----------------- Window size ----------------- */
		int width, height;
		glfwGetFramebufferSize(m_Window, &width, &height);

		// We use glfwGetProcAddress to find the function since it's an extension in 3.3
		//auto debugCallbackConfig = (void(APIENTRY*)(GLDEBUGPROC, const void*))glfwGetProcAddress("glDebugMessageCallback");
		//if (debugCallbackConfig) {
		//	glEnable(GL_DEBUG_OUTPUT);
		//	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); // Forces the error to happen on the current line
		//	debugCallbackConfig(MessageCallback, nullptr);
		//}
		

		/* ----------------- Scene ----------------- */
		Scene* initialScene = new Scene();

		SceneManager::Get().SetActiveScene(initialScene);

		/* ----------------- Example Objects ----------------- */
		MemoryManager::Get().CheckMemoryStatus();

	}

	void EngineContext::OnUpdate(float deltaTime) {

		m_Engine->Run();
		InputManager::Get().Update(deltaTime);
		SceneManager::Get().Update(deltaTime);
		MeshManager::Get().Update(deltaTime);
		ShaderManager::Get().Update(deltaTime);
		TextureManager::Get().Update(deltaTime);

		glGetError(); // Clear OpenGL errors each frame
	}

	void EngineContext::OnEnd() {
		SE_LOG(Engine, "EngineContext::OnEnd() - Shutting down EngineContext!");

		MeshManager::Get().Shutdown();

		m_Engine->Shutdown();

		if (m_Renderer) { delete m_Renderer; m_Renderer = nullptr; }

		m_Window = nullptr;
	}

}
