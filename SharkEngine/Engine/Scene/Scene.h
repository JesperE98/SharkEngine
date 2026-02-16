#ifndef SCENE_H
#define SCENE_H

#include "Core/GameObject.h"

namespace Shark::Components { class CameraController; class CameraComponent; }
namespace Shark::Core { class GameObject; }

namespace Shark {

	class Scene
	{
	public:
		Scene() = default;
		~Scene();

		void AddGameObject(Shark::Core::GameObject* obj);
		std::vector<Shark::Core::GameObject*>& GetGameObjects();

		void Update(float deltaTime);

		void CreateCamera(const float aspectRatio);
		Shark::Components::CameraComponent* GetCamera() const { return m_MainCamera; }

	private:
		std::vector<Shark::Core::GameObject*> m_GameObjects;

		Shark::Components::CameraComponent* m_MainCamera{ nullptr };
		Shark::Components::CameraController* m_CameraController{ nullptr };
	};
}


#endif SCENE_H