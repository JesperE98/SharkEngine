#pragma region EditorApp Libraries
#include "EditorApp.h"
#include "Managers/LevelEditorManager.h"
#include "Managers/WindowManager.h"
#include "EditorMenuBar.h"
#pragma endregion

#pragma region Engine Libraries
#include <Core/Engine/EngineContext.h>
#include <Core/App/EditorStateManager.h>
#include <Core/Utilities/Debug.h>
#include <Core/GameObject.h>

#include <Memory/MemoryManager.h>

#include <Scene/SceneManager.h>
#include <Scene/Scene.h>

#include <Graphics/Resources/MeshManager.h>
#include <Graphics/Resources/ShaderManager.h>
#include <Graphics/Resources/TextureManager.h>
#include <Graphics/Resources/PrimitiveMesh.h>
#include <Graphics/Rendering/ForwardRenderer.h>

#include <Components/UI/MainMenuComponent.h>
#include <Components/UI/LevelTimer.h>
#pragma endregion

#pragma region ImGUI libraries
#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_glfw.h>
#include <ImGui/imgui_impl_opengl3.h>
#pragma endregion

#include "GLFW/glfw3.h"

namespace Shark {

	using Core::EngineContext;
	using Core::Debug;
	using Core::Time;
	using Core::SceneManager;
	using Core::EditorStateManager;
	using Memory::MemoryManager;
	using Editor::LevelEditorManager;
	using Editor::WindowManager;
	using Editor::EditorMenuBar;
	using Resources::MeshManager;
	using Resources::ShaderManager;
	using Resources::TextureManager;
	using Graphics::PrimitiveType;

	void EditorApp::OnInitialize() {
		SE_PROC(Editor, "Initializing Editor App...");
		InitImGui();

		m_MenuBar = new EditorMenuBar();
		LevelEditorManager::Get().Initialize();
		WindowManager::Get().Initialize();

		MeshManager::Get().SetResponseTarget(&LevelEditorManager::Get().inbox);
		ShaderManager::Get().SetResponseTarget(&LevelEditorManager::Get().inbox);
		TextureManager::Get().SetResponseTarget(&LevelEditorManager::Get().inbox);

		LevelEditorManager::Get().RequestModelLoad("Models/Viking_House.obj");
		LevelEditorManager::Get().RequestPrimitiveLoad(PrimitiveType::Cube);
		
		SE_SUCC(Editor, "Editor App setup complete!");
	}

	void EditorApp::OnUpdate(float deltaTime) {
		LevelEditorManager::Get().Update(deltaTime);
		WindowManager::Get().Update(deltaTime);
	}

	void EditorApp::OnRender() {
		BeginImGuiFrame();
		WindowManager::Get().RenderWindows(0.0f); // deltaTime unused for UI
		RenderPlayBar();

		// Fix for main menu UI 
		// TODO: Later implement a more solvable solution to this issue
		auto* scene = SceneManager::Get().GetActiveScene();
		if (scene) {
			for (auto* obj : scene->GetGameObjects()) {
				if (auto* menu = obj->GetComponent<Components::MainMenuComponent>()) {
					menu->DrawUI();
				}
				if (auto* timer = obj->GetComponent<Components::LevelTimer>()) {
					timer->DrawHUD();
				}
			}
		}
		RenderFrustumCullingStatsWindow();

		ImGui::End();
		EndImGuiFrame();
	}

	void EditorApp::OnShutdown()
	{
		if (m_MenuBar) {
			delete m_MenuBar;
			m_MenuBar = nullptr;
		}

		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void EditorApp::InitImGui()
	{
		GLFWwindow* window = EngineContext::Get().GetWindow();

		if (!window) {
			SE_FAT(Editor, "Window was not created before CreateEditorWindow was called!");
			return;
		}

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		ImGuiStyle& style = ImGui::GetStyle();
		style.Colors[ImGuiCol_WindowBg] = ImVec4(0.25f, 0.25f, 0.25f, 1.0f);
		style.ItemSpacing.x = 50;
		style.ItemInnerSpacing.x = 5;

		ImGui_ImplGlfw_InitForOpenGL(window, true);

		ImGui_ImplOpenGL3_Init("#version 330");
	}

	void EditorApp::BeginImGuiFrame() {
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// Create a full screen dock space
		ImGuiWindowFlags flags =
			ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus |
			ImGuiWindowFlags_NoNavFocus;

		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);

		ImGui::PushStyleColor(ImGuiCol_MenuBarBg, ImVec4(1, 1, 1, 1));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);

		ImGui::Begin("Dockspace", nullptr, flags);
		ImGui::PopStyleVar(2);
		ImGui::PopStyleColor(1);

		m_MenuBar->OnImGuiRender();

		// Dockspace node
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);
	}

	void EditorApp::EndImGuiFrame()
	{
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
			GLFWwindow* backup = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup);
		}
	}
	void EditorApp::RenderPlayBar()
	{
		ImGui::Begin("Toolbar");
		EditorStateManager& sm = EditorStateManager::Get();

		if (ImGui::Button("Edit")) sm.OnSetState(EditorState::Edit);
		ImGui::SameLine();
		if (ImGui::Button("Play")) sm.OnSetState(EditorState::Play);
		ImGui::SameLine();
		if (ImGui::Button("Pause")) sm.OnSetState(EditorState::Pause);
		ImGui::SameLine();

		ImGui::Text("State: %s",
			sm.IsPlaying() ? "Playing" :
			sm.IsPaused() ? "Paused" : "Editing");

		ImGui::End();
	}

	void EditorApp::RenderFrustumCullingStatsWindow() {
		ImGui::SetNextWindowPos(ImVec2(10, 60), ImGuiCond_Always);
		ImGui::SetNextWindowBgAlpha(0.4f);

		ImGuiWindowFlags flags =
			ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize |
			ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings;

		ImGui::Begin("##CullStats", nullptr, flags);

		auto* renderer = static_cast<Graphics::ForwardRenderer*>(Core::EngineContext::Get().GetRenderer());

		if (renderer) {
			ImGui::Text("Drawn Objects: %d | Culled Objects: %d", renderer->GetDrawnCount(), renderer->GetCulledCount());
		}
		ImGui::End();
	}
}
