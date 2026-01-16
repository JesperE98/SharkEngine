#ifndef SCENE_H
#define SCENE_H

#include "Core/Entities/GameObject.h"
#include "Core/Entities/Camera.h"

class CameraController;

class Scene
{
public:
	Scene() = default;
	~Scene();

	void AddGameObject(GameObject* obj);
	std::vector<GameObject*>& GetGameObjects();

	void Update(float deltaTime);

	void CreateCamera(const float aspectRatio);
	Camera* GetCamera() const { return m_Camera; }

private:
	std::vector<GameObject*> gameObjects;

	Camera* m_Camera{ nullptr };
	CameraController* m_CameraController{ nullptr };
};

#endif SCENE_H