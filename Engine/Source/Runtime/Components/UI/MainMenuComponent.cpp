#include "MainMenuComponent.h"
#include "Scene/SceneManager.h"
#include "Core/RecordsManager.h"
#include "Core/Messaging/MessageSystem.h"

#include <ImGui/imgui.h>

namespace Shark::Components {
	
	using Core::SceneManager;
	using Core::RecordsManager;

	MainMenuComponent::MainMenuComponent() {
		
		tickMode = TickMode::Always;
	}

	void MainMenuComponent::Update(float deltaTime) {
		
	}

	void MainMenuComponent::DrawUI() {
		ImGuiViewport* vp = ImGui::GetMainViewport();
		ImVec2 center = ImVec2(vp->WorkPos.x + vp->WorkSize.x * 0.5f,
							   vp->WorkPos.y + vp->WorkSize.y * 0.5f);

		ImGui::SetNextWindowPos(center, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
		ImGui::SetNextWindowSize(ImVec2(400, 350), ImGuiCond_Always);

		ImGui::Begin("Shark Platformer", nullptr,
					 ImGuiWindowFlags_NoCollapse |
					 ImGuiWindowFlags_NoResize |
					 ImGuiWindowFlags_NoMove);

		ImGui::Text("Select a level");
		ImGui::Separator();
		ImGui::Spacing();

		static const LevelEntry levels[] = {
			{"Level 1", "Scenes/Level1.json"},
			{"Level 2", "Scenes/Level2.json"},
			{"Level 3", "Scenes/Level3.json"},
		};

		for (const auto& lvl : levels) {
			if (ImGui::Button(lvl.display, ImVec2(150, 0))) {
				Core::Message msg;
				msg.type = Core::EventType::LoadScene;
				msg.payload = lvl.path;

				SceneManager::Get().inbox.Push(msg);
			}

			ImGui::SameLine();

				if (RecordsManager::Get().HasRecord(lvl.display)) {
					float best = RecordsManager::Get().GetBestTime(lvl.display);
					ImGui::Text("Best: %.2fs", best);
				} else {
					ImGui::TextDisabled("No record yet");
				}
		}

		ImGui::Spacing();
		ImGui::Separator();
		if (ImGui::Button("Quit", ImVec2(150, 0))) {
			// TODO: signal engine to close
		}

		ImGui::End();
	}
}