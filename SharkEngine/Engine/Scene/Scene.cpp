#include "Scene.h"
#include "Components/Logic/CameraController.h"
#include "Components/Logic/CameraComponent.h"

namespace Shark {

    using Shark::Core::GameObject;
	using Shark::Components::CameraController;
	using Shark::Components::CameraComponent;

    Scene::~Scene()
    {
        if (m_CameraController) {
            delete m_CameraController;
            m_CameraController = nullptr;
        }

        for (auto& obj : m_GameObjects) {
            delete obj;
            obj = nullptr;
        }

        m_GameObjects.clear();
    }

    void Scene::Update(float deltaTime) {

        for (GameObject* obj : m_GameObjects) {
            obj->Tick(deltaTime);
        }

        // Or alternatively, render all IRenderables
        //for (auto& renderable : m_Renderables) {
        //    renderable->Draw();
        //}
    }

    void Scene::AddGameObject(GameObject* obj)
    {
        m_GameObjects.push_back(obj);
    }

    std::vector<GameObject*>& Scene::GetGameObjects()
    {
        return m_GameObjects;
    }

    void Scene::CreateCamera(const float aspectRatio)
    {
        GameObject* cam = new GameObject("Main Camera");
        cam->AddComponent<CameraComponent>(45.0f, aspectRatio, 0.1f, 1000.0f);
		cam->AddComponent<CameraController>(5.0f, 0.1f);
		cam->GetTransform().position = { 0.0f, 0.0f, 5.0f };

        m_MainCamera = cam->GetComponent<CameraComponent>();
        AddGameObject(cam);
    }

}
