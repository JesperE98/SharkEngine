#ifndef SCENE_H
#define SCENE_H

#include "Entities/GameObject.h"
#include "Entities/Camera.h"

namespace Shark::Entities {
	class GameObject;
	class Camera;
	class CameraController;
}

namespace Shark {

	class Scene
	{
	public:
		Scene() = default;
		~Scene();

		void AddGameObject(Shark::Entities::GameObject* obj);
		std::vector<Shark::Entities::GameObject*>& GetGameObjects();

		void Update(float deltaTime);

		void CreateCamera(const float aspectRatio);
		Shark::Entities::Camera* GetCamera() const { return m_Camera; }

	private:
		std::vector<Shark::Entities::GameObject*> gameObjects;

		Shark::Entities::Camera* m_Camera{ nullptr };
		Shark::Entities::CameraController* m_CameraController{ nullptr };
	};
}


#endif SCENE_H