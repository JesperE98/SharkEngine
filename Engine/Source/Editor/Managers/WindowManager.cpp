#include "WindowManager.h"
#include "LevelEditorManager.h"
#include "EditorWindows/HierarchyWindow.h"
#include "EditorWindows/InspectorWindow.h"
#include "EditorWindows/ConsoleWindow.h"
#include "EditorWindows/ContentBrowser.h"
#include "Viewport/SceneViewport.h"

#pragma	region Engine Includes
#include <Core/Engine/EngineContext.h>
#include <Graphics/Rendering/ForwardRenderer.h>
#include <Graphics/Rendering/Passes/ShadowMapPass.h>
#include <Scene/SceneManager.h>
#include <Memory/MemoryManager.h>
#include <Components/Rendering/LightComponent.h>
#pragma endregion

#include <vector>
#include <ImGui/imgui.h>

namespace Shark::Editor {

	using Core::EngineContext;
	using Core::EventType;
	using Core::Message;
	using Core::Debug;
	using Core::SceneManager;
	using Editor::LevelEditorManager;
	using Memory::MemoryManager;
	using Graphics::ForwardRenderer;
	using Graphics::ShadowMapPass;


	void WindowManager::Initialize()
	{
		// Creating Windows and setting up pointers for inter-window communication
		ConsoleWindow* consoleWindow = CreateEditorWindow<ConsoleWindow>();
		HierarchyWindow* hierarchyWindow = CreateEditorWindow<HierarchyWindow>();
		InspectorWindow* inspectorWindow = CreateEditorWindow<InspectorWindow>();
		ContentBrowser* contentBrowser = CreateEditorWindow<ContentBrowser>();
		m_SceneViewport = new SceneViewport("Scene");

		hierarchyWindow->SetInspector(inspectorWindow);

		// Set the inspector window in the LevelEditorManager so it can update it based on messages
		LevelEditorManager::Get().SetInspectorWindow(*inspectorWindow);

		for (auto& windows : m_EditorWindows) {
			windows->OnInitialize();
		}

		m_SceneViewport->OnInitialize();
	}

	void WindowManager::Update(float deltaTime) {
		Message msg;

		while(inbox.Pop(msg)) {
			ReceiveMessages(msg);
		}
	}

	void WindowManager::RenderWindows(float deltaTime)
	{
		Debug::CheckGLErrors("WindowManager::RenderWindows() - Start of Render");

		// Render all visible windows except the SceneViewport (which is rendered separately)
		for (auto& window : m_EditorWindows) {
			if (window->IsVisible()) {
				window->Render(deltaTime);
				Debug::CheckGLErrors("After Window: " + window->GetWindowName());
			}
		}

		// Render the SceneViewport last, as it typically contains the main 3D view of the editor
		if (m_SceneViewport) {
			RenderSceneViewport();
			Debug::CheckGLErrors("WindowManager::RenderWindows() - After Render Scene Viewport");
		}

		RenderStatsWindow();

		Debug::CheckGLErrors("WindowManager::RenderWindows() - End of Render");
	}

	void WindowManager::Shutdown()
	{
		for (EditorWindow* window : m_EditorWindows) {
			if(window) {
				window->OnShutdown();
				delete window;
			}
		}

		if(m_SceneViewport) {
			delete m_SceneViewport;
			m_SceneViewport = nullptr;
		}

		m_EditorWindows.clear();
	}

	void WindowManager::AddWindow(EditorWindow* window) {
		m_EditorWindows.push_back(window);
	}

	void WindowManager::RenderSceneViewport()
	{
		ImGui::Begin(m_SceneViewport->GetName().c_str());

		// Check for focus/hover state
		bool focused = ImGui::IsWindowFocused();
		bool hovered = ImGui::IsWindowHovered();
		m_SceneViewport->IsFocused(focused);
		m_SceneViewport->IsHovered(hovered);

		// Resize framebuffer to match ImGui content region
		ImVec2 avail = ImGui::GetContentRegionAvail();
		m_SceneViewport->SetSize(static_cast<int>(avail.x), static_cast<int>(avail.y));

		// Render the scene into viewport's framebuffer
		m_SceneViewport->OnRender(SceneManager::Get().GetActiveScene(), EngineContext::Get().GetRenderer());

		ImTextureID tex = (ImTextureID)(intptr_t)m_SceneViewport->GetColorAttachment();
		ImGui::Image(tex, avail, ImVec2(0, 1), ImVec2(1, 0));

		ImGui::End();
	}

	void WindowManager::ReceiveMessages(const Message& msg)
	{
		switch(msg.type) {
		case EventType::EditorWindowClose:
			for (auto& window : m_EditorWindows) {
				if(window->GetWindowName() == msg.payload) {
					window->SetWindowVisible(false);
					break;
				}
			}
			break;

			case EventType::EditorWindowOpen:
				for (auto& window : m_EditorWindows) {
					if(window->GetWindowName() == msg.payload) {
						window->SetWindowVisible(true);
						break;
					}
				}
			break;

		}
	}

	void WindowManager::RenderStatsWindow()
	{
		ImGui::Begin("Engine Statistics");

		auto& mem = MemoryManager::Get();

		// Periodically refresh the data (maybe every 100 frames so I don't spam the API)
		if (ImGui::GetFrameCount() % 60 == 0) {
			mem.CheckMemoryStatus();
		}

		ImGui::Text("Physical Memory Status:");
		ImGui::Text("Available: %llu MB", mem.GetAvailableMemory());
		ImGui::Text("Total:		%llu MB", mem.GetTotalMemory());

		// Adds a nice visual bar for portfolio
		float usage = 1.0f - (static_cast<float>(mem.GetAvailableMemory()) / static_cast<float>(mem.GetTotalMemory()));

		// --- Dynamic Color Logic ---

		ImVec4 color;
		if (usage < 0.70f) {
			color = ImVec4(0.2f, 0.8f, 0.2f, 1.0f); // Healthy Green
		}
		else if(usage < 0.90f) {
			color = ImVec4(0.9f, 0.6f, 0.1f, 1.0f); // Warning Orange
		}
		else {
			color = ImVec4(0.9f, 0.1f, 0.1f, 1.0f); // Crititcal Red
		}

		ImGui::PushStyleColor(ImGuiCol_PlotHistogram, color);

		ImGui::ProgressBar(usage, ImVec2(0, 0), "RAM Usage");

		ImGui::PopStyleColor();

		ImGui::End();
	}

#pragma region Template Implementations

	template<typename T>
	T* WindowManager::GetWindow() {
		for (auto* window : m_EditorWindows) {
			if (T* target = dynamic_cast<T*>(window)) {
				return target;
			}
		}
		return nullptr;
	}
#pragma	endregion
}