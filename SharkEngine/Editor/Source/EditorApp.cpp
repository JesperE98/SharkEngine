#pragma region EditorApp Libraries
#include "EditorApp.h"
#include "EditorWindows/WindowBase.h"
#include "EditorWindows/HierarchyWindow.h"
#include "EditorWindows/InspectorWindow.h"
#include "EditorWindows/ConsoleWindow.h"
#include "Viewport/SceneViewport.h"
#include "Managers/LevelEditorManager.h"
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
	using Shark::Core::Time;
	using Shark::Managers::MemoryManager;
	using Shark::Managers::SceneManager;
	using Shark::Editor::SceneViewport;
	using Shark::Editor::HierarchyWindow;
	using Shark::Editor::InspectorWindow;
	using Shark::Editor::ConsoleWindow;
	using Shark::Editor::LevelEditorManager;

	EditorApp::EditorApp()
	{
		std::unique_ptr<ConsoleWindow> consoleWindow = std::make_unique<ConsoleWindow>();
		std::unique_ptr<HierarchyWindow> hierarchyWindow = std::make_unique<HierarchyWindow>();
		std::unique_ptr<InspectorWindow> inspectorWindow = std::make_unique<InspectorWindow>();

		// Creating Windows and setting up pointers for inter-window communication
		m_ConsoleWindow = consoleWindow.get();
		m_HierarchyWindow = hierarchyWindow.get();
		m_InspectorWindow = inspectorWindow.get();

		LevelEditorManager::Get().SetInspectorWindow(*m_InspectorWindow);

		m_Windows.push_back(std::move(consoleWindow));
		m_Windows.push_back(std::move(hierarchyWindow));
		m_Windows.push_back(std::move(inspectorWindow));

		m_HierarchyWindow->SetInspector(m_InspectorWindow);

		for (auto& panels : m_Windows) {
			panels->OnInitialize();
		}

		SE_LOG(Editor, "EditorApp::Edtior() - Created EditorApp.");
		EngineContext::Get().OnInitialize();
	}

	EditorApp::~EditorApp()
	{
		for(auto& window : m_Windows){
			window->OnShutdown();
		}

		m_Windows.clear();
		m_SceneViewport.reset();

		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void EditorApp::Run() {
		CreateEditorWindow();
		GLFWwindow* window = EngineContext::Get().m_Window;

		float deltaTime;

		Time::GetLastFrame();

		while (!glfwWindowShouldClose(EngineContext::Get().m_Window))
		{
			glfwPollEvents();

			// Current Time
			Time::GetCurrentFrame();

			// Time difference between frames
			deltaTime = static_cast<float>(Time::GetDeltaTime());

			LevelEditorManager::Get().Update(deltaTime);
			// Update Engine + scene logic
			EngineContext::Get().OnUpdate(deltaTime);

			BeginFrame();

			// Panels + viewport UI
			RenderPanels(deltaTime);

			m_SceneViewport->UpdateViewportSize();

			// Render into SceneViewport framebuffer
			m_SceneViewport->OnRender(*SceneManager::Get().GetActiveScene(), *EngineContext::Get().m_Renderer);

			// Render the stats window
			RenderStatsWindow();

			RenderSceneViewport();

			// ImGui final render
			Render();
			glfwSwapBuffers(window);
		}

		EngineContext::Get().OnEnd();
	}

	void EditorApp::CheckGLErrors(const std::string& context)
	{
		GLenum err;
		while ((err = glGetError()) != GL_NO_ERROR) {
			SE_ERR(OpenGL, "EditorApp::CheckGlErrors() - {}: OpenGL error: {}", context, err);
		}

	}

	void EditorApp::CreateEditorWindow()
	{
		glfwMakeContextCurrent(EngineContext::Get().m_Window);	//// Just a dummy VAO for OpenGL 3.3 core profile

		SE_LOG(Editor, "EditorApp::CreateEditorWindow() - Creating EditorApp Window.");

		const GLubyte* version = glGetString(GL_VERSION);
		SE_LOG(OpenGL, "OpenGL version: {}", version);

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		CheckGLErrors("Error after VAO: ");

		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		ImGuiStyle& style = ImGui::GetStyle();
		style.Colors[ImGuiCol_WindowBg] = ImVec4(0.25f, 0.25f, 0.25f, 1.0f);
		style.ItemSpacing.x = 50;
		style.ItemInnerSpacing.x = 5;
		//ImGui::StyleColorsLight();

		ImGui_ImplGlfw_InitForOpenGL(EngineContext::Get().m_Window, true);
		CheckGLErrors("Error after ImGui_ImplGlfw_InitForOpenGL: ");

		ImGui_ImplOpenGL3_Init("#version 330");
		CheckGLErrors("Error after ImGui_ImplOpenGL3_Init: ");

		InitializeViewport();
	}

	void EditorApp::InitializeViewport()
	{
		// Create SceneViewport
		m_SceneViewport = std::make_unique<SceneViewport>("Scene");
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

		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);

		ImGui::Begin("Dockspace Demo", nullptr, window_flags);
		ImGui::PopStyleVar(2);

		// Dockspace node
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

		ImGui::End();
	}

	void EditorApp::RenderPanels(float deltaTime)
	{
		GLenum err;
		while ((err = glGetError()) != GL_NO_ERROR) {
			SE_ERR(OpenGL, "EditorApp::RenderPanels() - OpenGL error before rendering panels: {}", err);
		}

		for (auto& panel : m_Windows) {
			if (panel->IsVisible()) {
				panel->OnRenderPanel(deltaTime);
			}
		}

		while ((err = glGetError()) != GL_NO_ERROR) {
			SE_ERR(OpenGL, "EditorApp::RenderPanels() - OpenGL error after rendering panels: {}", err);
		}
	}

	void EditorApp::RenderSceneViewport()
	{
		ImGui::Begin(m_SceneViewport->GetName().c_str());

		ImVec2 avail = ImGui::GetContentRegionAvail();
		ImTextureID tex = (ImTextureID)(intptr_t)m_SceneViewport->GetColorAttachment();

		ImGui::Image(tex, avail, ImVec2(0, 1), ImVec2(1, 0));

		ImGui::End();
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
	void EditorApp::RenderStatsWindow()
	{
		ImGui::Begin("Engine Statistics");

		auto& mem = MemoryManager::Get();

		// Periodically refresh the data (maybe every 100 frames so I don't spam the API)
		if (ImGui::GetFrameCount() % 60 == 0){
			mem.CheckMemoryStatus();
		}

		ImGui::Text("Physical Memory Status:");
		ImGui::Text("Available: %llu MB", mem.GetAvailableMemory());
		ImGui::Text("Total:		%llu MB", mem.GetTotalMemory());

		// Adds a nice visual bar for portfolio
		float usage = 1.0f - (static_cast<float>(mem.GetAvailableMemory()) / static_cast<float>(mem.GetTotalMemory()));
		ImGui::ProgressBar(usage, ImVec2(0, 0), "RAM Usage");

		ImGui::End();
	}
}
