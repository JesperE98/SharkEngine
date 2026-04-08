#ifndef SCENE_H
#define SCENE_H

#include "Core/GameObject.h"

namespace Shark::Components { class CameraController; class CameraComponent; }
namespace Shark::Core { class GameObject; }

namespace Shark {

	class Scene
	{
	public:
		Scene();
		~Scene();

		void AddGameObject(Shark::Core::GameObject* obj);
		const std::vector<Shark::Core::GameObject*>& GetGameObjects();
		void DestroyGameObject(Shark::Core::GameObject* obj);

		void Update(float deltaTime);

		void CreateCamera();
		void CreateDirectionalLight();
		void CreatePointLight();
		Shark::Components::CameraComponent* GetCamera() const;

	private:
		std::vector<Shark::Core::GameObject*> m_GameObjects;
		std::vector<Shark::Core::GameObject*> m_ObjectsToDestroy;
		Shark::Components::CameraController* m_CameraController{ nullptr };
	};
}


#endif SCENE_H