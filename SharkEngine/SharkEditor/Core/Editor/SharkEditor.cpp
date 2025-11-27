#pragma region Editor Libraries
#include "SharkEditor.h"
#include "Source/Panels/HierarchyPanel.h"
#include "Source/Panels/InspectorPanel.h"
#include "Source/Viewport/SceneViewport.h"
#pragma endregion

#pragma region Engine Libraries
#include <Core/Engine/EngineContext.h>
#include <Core/Utilities/Time.h>
#pragma endregion

#pragma region ImGUI libraries
#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_glfw.h>
#include <ImGui/imgui_impl_opengl3.h>
#pragma endregion

#include <iostream>

SharkEditor::SharkEditor()
{
	std::cout << Time::CreateTimeStamp() << ": " << "SharkEditor created.\n";
	EngineContext::Get().OnInit();
	const GLubyte* version = glGetString(GL_VERSION);
	std::cout << Time::CreateTimeStamp() << ": " << "OpenGL version: " << version << std::endl;
}

SharkEditor::~SharkEditor()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void SharkEditor::Run() {
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

		BeginFrame();

		// Update Engine + scene logic
		EngineContext::Get().OnUpdate(deltaTime);

		// Render into SceneViewport framebuffer
		m_SceneViewport->OnRender(*EngineContext::Get().m_Scene, *EngineContext::Get().m_Renderer);

		// Panels + viewport UI
		RenderPanels(deltaTime);
		RenderSceneViewport();

		// ImGui final render
		Render(); 

		glfwSwapBuffers(window);
	}

	EngineContext::Get().OnEnd();
}

void SharkEditor::CheckGLErrors(const std::string& context)
{
	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR) {
		std::cout << Time::CreateTimeStamp() << ": " << context << err << std::endl;
	}
	
}

void SharkEditor::CreateEditorWindow()
{
	glfwMakeContextCurrent(EngineContext::Get().m_Window);	//// Just a dummy VAO for OpenGL 3.3 core profile


	std::cout << Time::CreateTimeStamp() << ": " << "Creating Editor Window.\n";

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

	InitPanels();
}

void SharkEditor::InitPanels()
{
	// Creating Panels
	m_HierarchyPanel = new HierarchyPanel();
	m_InspectorPanel = new InspectorPanel();

	m_Panels.emplace_back(m_HierarchyPanel);
	m_Panels.emplace_back(m_InspectorPanel);

	m_HierarchyPanel->SetInspector(m_InspectorPanel);
	
	for (auto& panels : m_Panels) {
		panels->OnInit();
	}

	// Create SceneViewport
	m_SceneViewport = std::make_unique<SceneViewport>("Scene");
}

void SharkEditor::BeginFrame(){
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

void SharkEditor::RenderPanels(float deltaTime)
{
	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR) {
		std::cout << Time::CreateTimeStamp() << ": " << "OpenGL error: " << err << std::endl;
	}
	for (auto& panel : m_Panels) {
		if (panel->IsVisible()) {
			panel->OnRenderPanel(deltaTime);
		}
	}

	while ((err = glGetError()) != GL_NO_ERROR) {
		std::cout << Time::CreateTimeStamp() << ": " << "After HierarchyPanel render: OpenGL error: " << err << std::endl;
	}
}

void SharkEditor::RenderSceneViewport()
{
	ImGui::Begin(m_SceneViewport->GetName().c_str());

	ImVec2 avail = ImGui::GetContentRegionAvail();
	m_SceneViewport->SetSize(static_cast<int>(avail.x), static_cast<int>(avail.y));

	ImTextureID tex = (ImTextureID)(intptr_t)m_SceneViewport->GetColorAttachment();
	ImGui::Image(tex, avail, ImVec2(0, 1), ImVec2(1, 0));

	ImGui::End();
}

void SharkEditor::Render(){
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR) {
		std::cout << Time::CreateTimeStamp() << ": " << "OpenGL error after ImGui: " << err << std::endl;
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