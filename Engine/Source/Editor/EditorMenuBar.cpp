#include "EditorMenuBar.h"
#include "Managers/WindowManager.h"

#include <Core/Serialization/SceneSerializer.h>
#include <Scene/SceneManager.h>
#include <Scene/Scene.h>
#include <Core/GameObject.h>
#include <Core/Spatial/OctreeSystem.h>
#include <Components/Physics/AABBComponent.h>
#include <Graphics/Resources/MeshManager.h>
#include <Graphics/Resources/PrimitiveMesh.h>
#include <Components/Rendering/MeshRendererComponent.h>
#include <ImGui/imgui.h>


namespace Shark::Editor {

	using Shark::Scene;
	using Serialization::SceneSerializer;
	using Core::SceneManager;
	using Core::GameObject;
	using Spatial::OctreeSystem;
	using Resources::MeshManager;
	using Graphics::PrimitiveMesh;
	using Graphics::PrimitiveType;
	using Graphics::Mesh;
	using Graphics::Material;
	using Components::MeshRendererComponent;
	using Components::AABBComponent;

	void EditorMenuBar::OnImGuiRender()
	{
		// --- COLORS --- 
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.0f, 0.0f, 1.0f)); // Black text for better contrast
		ImGui::PushStyleColor(ImGuiCol_MenuBarBg, ImVec4(1.0f, 1.0f, 1.0f, 1.0f)); // White background for menu bars
		ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(1.0f, 1.0f, 1.0f, 1.0f)); // White background for popups
		ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.9f, 0.9f, 0.9f, 1.0f)); // Light gray for hovered menu items

		// --- STYLE ---
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10.0f, 4.0f)); // Increased padding for better clickability
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(15.0f, 4.0f)); // Increased spacing between items for better readability

		if (ImGui::BeginMenuBar()) {
			DrawFileMenu();
			DrawEditMenu();
			DrawViewMenu();
			DrawWindowsMenu();
			DrawSettingsMenu();
			DrawHelpMenu();


			if (ImGui::BeginMenu("Debug")) {
				if (ImGui::MenuItem("Spawn 50 Test Cubes")) {
					SpawnTestCubes(50);
				}
				if (ImGui::MenuItem("Spawn 500 Test Cubes")) {
					SpawnTestCubes(500);
				}
				if (ImGui::MenuItem("Clear Test Cubes")) {
					ClearTestCubes();
				}

				ImGui::Separator();

				bool drawOctree = OctreeSystem::Get().IsDebugDrawEnabled();
				if (ImGui::MenuItem("Show Octree WireFrame", nullptr, drawOctree)) {
					OctreeSystem::Get().SetDebugDrawEnabled(!drawOctree);
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		ImGui::PopStyleVar(2); // Pop FramePadding and ItemSpacing
		ImGui::PopStyleColor(4); // Pop the 5 colors we pushed
	}

	void EditorMenuBar::DrawFileMenu()
	{
		if(ImGui::BeginMenu("File")){
			if (ImGui::MenuItem("New Scene", "Ctrl+N")) { /* TODO: SceneManager::Get().CreateNewScene(); */ }
			if (ImGui::MenuItem("Load Scene", "Ctrl+O")) { 
				OnLoadScene();
			}
			ImGui::Separator();
			if (ImGui::MenuItem("Save", "Ctrl+S")) { 
				OnSaveScene();
			}
			ImGui::Separator();
			if (ImGui::MenuItem("Exit", "Alt+F4")) {
				// Accessing the Engine context to close the app
				/*glfwSetWindowShouldClose(EngineContext::Get().m_Window, true);*/
			}
			ImGui::EndMenu();
		}
	}

	void EditorMenuBar::DrawEditMenu()
	{
		if(ImGui::BeginMenu("Edit")) {
			if (ImGui::MenuItem("Undo", "Ctrl+Z")) { /* TODO: Implement Undo functionality */ }
			if (ImGui::MenuItem("Redo", "Ctrl+Y")) { /* TODO: Implement Redo functionality */ }
			ImGui::Separator();
			if (ImGui::MenuItem("Cut", "Ctrl+X")) { /* TODO: Implement Cut functionality */ }
			if (ImGui::MenuItem("Copy", "Ctrl+C")) { /* TODO: Implement Copy functionality */ }
			if (ImGui::MenuItem("Paste", "Ctrl+V")) { /* TODO: Implement Paste functionality */ }
			ImGui::EndMenu();
		}
	}

	void EditorMenuBar::DrawViewMenu()
	{
		if (ImGui::BeginMenu("View")) {
			if (ImGui::MenuItem("Toggle Fullscreen", "F11")) { /* TODO: Implement Fullscreen toggle */ }
			ImGui::Separator();
			if (ImGui::MenuItem("Zoom In", "Ctrl++")) { /* TODO: Implement Zoom In functionality */ }
			if (ImGui::MenuItem("Zoom Out", "Ctrl+-")) { /* TODO: Implement Zoom Out functionality */ }
			ImGui::EndMenu();
		}
	}

	void EditorMenuBar::DrawWindowsMenu()
	{
		if(ImGui::BeginMenu("Windows")) {
			WindowManager& wm = WindowManager::Get();
			for (auto* window : wm.GetWindows()) {
				bool isVisible = window->IsVisible();
				if (ImGui::MenuItem(window->GetWindowName().c_str(), nullptr, isVisible)) {
					window->SetWindowVisible(!isVisible);
				}
			}
			ImGui::EndMenu();
		}
	}

	void EditorMenuBar::DrawSettingsMenu()
	{
		if (ImGui::BeginMenu("Settings")) {
			if (ImGui::MenuItem("Editor Style")){ /* TODO: Open Style Settings Window */ }
			if (ImGui::MenuItem("Preferences")) { /* TODO: Open Preferences Window */ }
			ImGui::EndMenu();
		}
	}

	void EditorMenuBar::DrawHelpMenu()
	{
		if (ImGui::BeginMenu("Help")) {
			if (ImGui::MenuItem("Documentation")) { /* TODO: Open Documentation URL */ }
			if (ImGui::MenuItem("Report a Bug")) { /* TODO: Open Bug Report URL */ }
			if (ImGui::MenuItem("About")) { /* TODO: Open About Window */ }
			ImGui::EndMenu();
		}
	}
	
	void EditorMenuBar::OnSaveScene() {
		Scene* scene = SceneManager::Get().GetActiveScene();
		if (scene) {
			std::string path = "Content/Scenes/" + scene->GetName() + ".json";
			SceneSerializer serializer(scene);
			serializer.SaveToFile(path);
		}
	}

	void EditorMenuBar::OnLoadScene() {
		std::string path = "Content/Scenes/DefaultScene.json";

		Scene* newScene = new Scene();

		SceneSerializer serializer(newScene);

		if (serializer.LoadFromFile(path)) {
			// Replace active scene
			SceneManager::Get().SetActiveScene(newScene);
		} else {
			delete newScene;
			SE_ERR(Editor, "Failed to load scene from {}", path);
		}
	}
	void EditorMenuBar::SpawnTestCubes(int count) {
		Scene* scene = SceneManager::Get().GetActiveScene();
		if (!scene) {
			SE_WARN(Editor, "No active scene to spawn cubes into.");
			return;
		}

		Mesh* cubeMesh = MeshManager::Get().LoadMesh(PrimitiveType::Cube);
		if (!cubeMesh) {
			SE_ERR(Editor, "Failed to load Cube Primitive");
			return;
		}

		for (int i = 0; i < count; ++i) {
			GameObject* obj = new GameObject("TestCube_" + std::to_string(i));

			// Random positions in [-30, 30]
			float x = ( rand() % 60 ) - 30.0f;
			float y = ( rand() % 20 ) - 10.0f;
			float z = ( rand() % 60 ) - 30.0f;
			
			obj->GetTransform().position = { x, y, z };
			obj->GetTransform().scale = { 1, 1, 1 };

			Material* mat = new Material();
			obj->AddComponent<MeshRendererComponent>(cubeMesh, mat);
			obj->AddComponent<AABBComponent>(Math::Vector3(0.5f, 0.5f, 0.5f), true);

			scene->AddGameObject(obj);
			OctreeSystem::Get().Insert(obj);
		}

		SE_LOG(Editor, "Spawned {} test cubes.", count);
	}

	void EditorMenuBar::ClearTestCubes() {
		Scene* scene = SceneManager::Get().GetActiveScene();
		if (!scene) return;

		int removed = 0;

		auto objects = scene->GetGameObjects();
		for (auto* obj : objects) {
			if (obj && obj->GetName().starts_with("TestCube_")) {
				OctreeSystem::Get().Remove(obj);
				scene->DestroyGameObject(obj);
				++removed;
			}
		}

		// Forces the octree to rebuild on the next frame after destroy queue drains
		OctreeSystem::Get().RebuildFromScene(scene);

		SE_LOG(Editor, "Removed {} test cubes.", removed);
	}
}