#pragma region EditorApp Libraries
#include "EditorApp.h"
#include "Managers/LevelEditorManager.h"
#include "Managers/WindowManager.h"
#include "EditorMenuBar.h"
#pragma endregion

#pragma region Engine Libraries
#include <Core/Engine/EngineContext.h>
#include <Core/Utilities/Debug.h>
#include <Managers/MemoryManager.h>
#include <Managers/SceneManager.h>
#pragma endregion

#pragma region ImGUI libraries
#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_glfw.h>
#include <ImGui/imgui_impl_opengl3.h>
#pragma endregion

namespace Shark {

	using Shark::Core::EngineContext;
	using Shark::Core::Debug;
	using Shark::Core::Time;
	using Shark::Managers::MemoryManager;
	using Shark::Managers::SceneManager;
	using Shark::Editor::LevelEditorManager;
	using Shark::Editor::WindowManager;
	using Shark::Editor::EditorMenuBar;

	EditorApp::~EditorApp()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void EditorApp::Run() {

		EngineContext::Get().OnInitialize();
		CreateEditorWindow();

		m_MenuBar = new EditorMenuBar();
		LevelEditorManager::Get().Initialize();
		WindowManager::Get().Initialize();

		GLFWwindow* window = EngineContext::Get().m_Window;

		while (!glfwWindowShouldClose(EngineContext::Get().m_Window))
		{
			glfwPollEvents();

			Time::Update(); // Update the time system to calculate delta time
			float deltaTime = Time::GetDeltaTime(); // Time difference between frames

			// --- Managers Update ---
			LevelEditorManager::Get().Update(deltaTime);
			WindowManager::Get().Update(deltaTime);
			EngineContext::Get().OnUpdate(deltaTime);

			BeginFrame(); // Start the ImGui frame and create the dockspace
			WindowManager::Get().RenderWindows(deltaTime); // Render the stats window

			ImGui::End();
			Render(); // ImGui final render
			glfwSwapBuffers(window);
		}

		EngineContext::Get().OnEnd();
	}



	void EditorApp::CreateEditorWindow()
	{
		glfwMakeContextCurrent(EngineContext::Get().m_Window);	//// Just a dummy VAO for OpenGL 3.3 core profile

		SE_LOG(Editor, "EditorApp::CreateEditorWindow() - Creating EditorApp Window.");

		const GLubyte* version = glGetString(GL_VERSION);
		SE_LOG(OpenGL, "OpenGL version: {}", version);

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		Debug::CheckGLErrors("Error after VAO: ");

		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		ImGuiStyle& style = ImGui::GetStyle();
		style.Colors[ImGuiCol_WindowBg] = ImVec4(0.25f, 0.25f, 0.25f, 1.0f);
		style.ItemSpacing.x = 50;
		style.ItemInnerSpacing.x = 5;
		//ImGui::StyleColorsLight();

		ImGui_ImplGlfw_InitForOpenGL(EngineContext::Get().m_Window, true);
		Debug::CheckGLErrors("Error after ImGui_ImplGlfw_InitForOpenGL: ");

		ImGui_ImplOpenGL3_Init("#version 330");
		Debug::CheckGLErrors("Error after ImGui_ImplOpenGL3_Init: ");

	}

	void EditorApp::BeginFrame() {
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// Create a full screen dock space
		ImGuiWindowFlags window_flags =
			ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus |
			ImGuiWindowFlags_NoNavFocus;

		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);

		ImGui::PushStyleColor(ImGuiCol_MenuBarBg, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);

		ImGui::Begin("Dockspace Demo", nullptr, window_flags);
		ImGui::PopStyleVar(2);
		ImGui::PopStyleColor(1);

		m_MenuBar->OnImGuiRender();
		// Dockspace node
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}

	void EditorApp::Render() {
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		GLenum err;
		while ((err = glGetError()) != GL_NO_ERROR) {
			SE_ERR(OpenGL, "EditorApp::Render() - OpenGL error after ImGui: {}", err);
		}

		// If multi-viewport enabled, update and render platform windows
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}
}
