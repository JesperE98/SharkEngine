#include "WindowManager.h"
#include "LevelEditorManager.h"
#include "Source/EditorWindows/HierarchyWindow.h"
#include "Source/EditorWindows/InspectorWindow.h"
#include "Source/EditorWindows/ConsoleWindow.h"
#include "Source/Viewport/SceneViewport.h"

#pragma	region Engine Includes
#include <Core/Engine/EngineContext.h>
#include <Graphics/Rendering/ForwardRenderer.h>
#include <Graphics/Rendering/Passes/ShadowMapPass.h>
#include <Managers/SceneManager.h>
#include <Managers/MemoryManager.h>
#include <Components/Rendering/LightComponent.h>
#pragma endregion

#include <vector>

namespace Shark::Editor {

	using Shark::Core::EngineContext;
	using Shark::Core::EventType;
	using Shark::Core::Message;
	using Shark::Core::Debug;
	using Shark::Editor::LevelEditorManager;
	using Shark::Managers::SceneManager;
	using Shark::Managers::MemoryManager;
	using Shark::Graphics::ForwardRenderer;
	using Shark::Graphics::ShadowMapPass;


	void WindowManager::Initialize()
	{
		// Creating Windows and setting up pointers for inter-window communication
		ConsoleWindow* consoleWindow = CreateEditorWindow<ConsoleWindow>();
		HierarchyWindow* hierarchyWindow = CreateEditorWindow<HierarchyWindow>();
		InspectorWindow* inspectorWindow = CreateEditorWindow<InspectorWindow>();
		m_SceneViewport = new SceneViewport("Scene");

		hierarchyWindow->SetInspector(inspectorWindow);

		// Set the inspector window in the LevelEditorManager so it can update it based on messages
		LevelEditorManager::Get().SetInspectorWindow(*inspectorWindow);

		for (auto& windows : m_Windows) {
			windows->OnInitialize();
		}

		m_SceneViewport->OnInitialize();

		SE_LOG(Editor, "EditorApp::Edtior() - Created EditorApp.");
	}

	void WindowManager::Update(float deltaTime) {
		Message msg;

		while(inbox.Pop(msg)) {
			RecieveMessages(msg);
		}
	}

	void WindowManager::RenderWindows(float deltaTime)
	{
		Debug::CheckGLErrors("WindowManager::RenderWindows() - Start of Render");

		// Render all visible windows except the SceneViewport (which is rendered separately)
		for (auto& window : m_Windows) {
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
		for (EditorWindow* window : m_Windows) {
			if(window) {
				window->OnShutdown();
				delete window;
			}
		}

		if(m_SceneViewport) {
			delete m_SceneViewport;
			m_SceneViewport = nullptr;
		}

		m_Windows.clear();
	}

	bool WindowManager::IsSceneViewportFocused() const
	{
		return m_SceneViewport->IsFocused();
	}

	bool WindowManager::IsSceneViewportHovered() const
	{
		return m_SceneViewport->IsHovered();
	}

	void WindowManager::AddWindow(EditorWindow* window) {
		m_Windows.push_back(window);
	}

	void WindowManager::RenderSceneViewport()
	{
		m_SceneViewport->UpdateViewportSize();
		m_SceneViewport->OnRender(*SceneManager::Get().GetActiveScene(), *EngineContext::Get().m_Renderer);

		ImGui::Begin(m_SceneViewport->GetName().c_str());

		ImVec2 avail = ImGui::GetContentRegionAvail();

		// Debug: Cast to ForwardRenderer to get the Shadow Texture ID
		auto* fr = dynamic_cast<ForwardRenderer*>(EngineContext::Get().m_Renderer);
		unsigned int debugTexID = 0;

		//if (fr && fr->GetShadowPass()) {
		//	debugTexID = fr->GetShadowPass()->GetShadowMapTexture();
		//}

		ImTextureID tex = (ImTextureID)(intptr_t)m_SceneViewport->GetColorAttachment();
		//ImTextureID tex = (ImTextureID)(intptr_t)debugTexID;

		ImGui::Image(tex, avail, ImVec2(0, 1), ImVec2(1, 0));

		ImGui::End();
	}

	void WindowManager::RecieveMessages(const Message& msg)
	{
		switch(msg.type) {
		case EventType::EditorWindowClose:
			for (auto& window : m_Windows) {
				if(window->GetWindowName() == msg.payload) {
					window->SetWindowVisible(false);
					break;
				}
			}
			break;

			case EventType::EditorWindowOpen:
				for (auto& window : m_Windows) {
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
		for (auto* window : m_Windows) {
			if (T* target = dynamic_cast<T*>(window)) {
				return target;
			}
		}
		return nullptr;
	}
#pragma	endregion
}