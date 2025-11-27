#ifndef ENGINE_CONTEXT_H
#define ENGINE_CONTEXT_H

#include "SharkEngine.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Renderer;
class Scene;

class EngineContext
{
public:
	// Singleton accessor
	static EngineContext& Get() {
		static EngineContext instance;
		return instance;
	}

#pragma region Core Engine objects
	SharkEngine* m_Engine{ nullptr };
	Scene* m_Scene{ nullptr };
	Renderer* m_Renderer{ nullptr };
	GLFWwindow* m_Window{ nullptr };
#pragma endregion

	// Initilizes engine, renderer, scene
	void OnInit();
	// Update engine + scene logic
	void OnUpdate(float deltaTime);
	// Shutdown and cleanup
	void OnEnd();

private:
	EngineContext() = default;
	~EngineContext() = default;

	EngineContext(const EngineContext&) = delete;
	EngineContext& operator = (const EngineContext&) = delete;


};

#endif