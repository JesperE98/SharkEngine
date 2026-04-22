#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

struct GLFWwindow;

namespace Shark::Input {
	class InputManager
	{
	public:
		static InputManager& Get();

		void Initialize(GLFWwindow* window);
		void Update(float deltaTime);

		bool IsKeyPressed(int key);
		bool IsMouseButtonPressed(int key);
		void GetMouseDelta(float& xOffset, float& yOffset);
		GLFWwindow* GetWindow() { return m_Window; }

		// Delete copy/assignment for Singleton safety
		InputManager(const InputManager&)				= delete;
		InputManager& operator=(const InputManager&)	= delete;
	private:
		GLFWwindow* m_Window = nullptr;
		double m_LastX{ 0.0f }, m_LastY{ 0.0f };
		float m_MouseDeltaX{ 0.0f }, m_MouseDeltaY{ 0.0f };
		bool m_FirstMouse{ true };

		InputManager() = default;
		~InputManager() = default;
	};

}

#endif // INPUTMANAGER_H