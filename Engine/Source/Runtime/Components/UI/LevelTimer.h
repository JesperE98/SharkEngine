#pragma once

#include "Components/SharkBehavior.h"

namespace Shark::Components {

	class LevelTimer : public SharkBehavior {
	public:
		LevelTimer() {
			tickMode = TickMode::PlayOnly;
		}

		void BeginPlay() override final;
		void Update(float deltaTime) override final;
		float GetElapsedTime() const { return m_Elapsed; }
		bool IsRunning() const { return m_Running; }

		void DrawHUD();
		void Stop() { m_Running = false; }
		void Reset() {
			m_Elapsed = 0.0f;
			m_Running = true;
		}

	private:

		float m_Elapsed = 0.0f;
		bool m_Running = true;
	};
}

