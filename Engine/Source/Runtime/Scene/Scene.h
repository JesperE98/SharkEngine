#ifndef SCENE_H
#define SCENE_H

#include <Core/Object.h>
#include <vector>

namespace Shark::Components { 
	class CameraController; 
	class CameraComponent; 
}

namespace Shark::Core {
	class GameObject;
}

namespace Shark {

	class Scene : public Object
	{
	public:
		Scene();
		~Scene();

		void AddGameObject(Core::GameObject* obj);
		const std::vector<Core::GameObject*>& GetGameObjects();
		void DestroyGameObject(Core::GameObject* obj);

		void Update(float deltaTime);

		void CreateCamera();
		void CreateDirectionalLight();
		void CreatePointLight();
		Components::CameraComponent* GetCamera() const;


	private:
		std::vector<Core::GameObject*> m_GameObjects;
		std::vector<Core::GameObject*> m_ObjectsToDestroy;
		Components::CameraController* m_CameraController{ nullptr };
	};
}


#endif SCENE_H