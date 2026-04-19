#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <glfw3.h>

namespace Shark::Managers {
	class InputManager
	{
	public:
		static InputManager& Get();

		void OnInitialize(GLFWwindow* window);
		void Update(float deltaTime);

		bool IsKeyPressed(int key);
		bool IsMouseButtonPressed(int key);
		void GetMouseDelta(float& xOffset, float& yOffset);
		GLFWwindow* GetWindow() { return m_Window; }

	private:
		InputManager() = default;
		~InputManager() = default;

		// Delete copy/assignment for Singleton safety
		InputManager(const InputManager&) = delete;
		InputManager& operator=(const InputManager&) = delete;

		GLFWwindow* m_Window{ nullptr };
		double m_LastX{ 0.0f }, m_LastY{ 0.0f };
		float m_MouseDeltaX{ 0.0f }, m_MouseDeltaY{ 0.0f };
		bool m_FirstMouse{ true };
	};

}

#endif // INPUTMANAGER_H