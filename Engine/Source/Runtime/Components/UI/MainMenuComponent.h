#pragma once

#include "Components/SharkBehavior.h"
#include <vector>

namespace Shark::Components {

	struct LevelEntry {
		const char* display;
		const char* path;
	};

	class MainMenuComponent : public SharkBehavior {
	public:
		MainMenuComponent();

		void Update(float deltaTime) override final;
		void DrawUI();
	};
}