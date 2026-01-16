#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <GLFW/glfw3.h>

class InputManager
{
public:
	static void Init(GLFWwindow* window);
	static void Update(float deltaTime);
	static void GetMouseDelta(float& xOffset, float& yOffset);

	static bool IsKeyPressed(int key);
	static bool IsMouseButtonPressed(int key);
	static GLFWwindow* GetWindow() { return m_Window; }

private:
	static GLFWwindow* m_Window;
	static double m_LastX, m_LastY;
	static float m_MouseDeltaX, m_MouseDeltaY;
	static bool m_FirstMouse;
};

#endif // INPUTMANAGER_H