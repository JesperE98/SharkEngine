#include "Scene.h"
#include "Components/Logic/CameraController.h"
#include "Components/Logic/CameraComponent.h"
#include "Components/Rendering/LightComponent.h"

namespace Shark {

    using Shark::Core::GameObject;
	using Shark::Components::CameraController;
	using Shark::Components::CameraComponent;
    using Shark::Components::LightComponent;
    using Shark::Components::LightType;

    Scene::Scene()
    {
		CreateCamera();
        CreateDirectionalLight();
    }

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

        for (GameObject* obj : m_ObjectsToDestroy) {
			auto it = std::find(m_GameObjects.begin(), m_GameObjects.end(), obj);
			if (it != m_GameObjects.end()) {
                delete *it;
                m_GameObjects.erase(it);
            }
        }
    }

    void Scene::AddGameObject(GameObject* obj)
    {
        m_GameObjects.push_back(obj);
    }

    std::vector<GameObject*>& Scene::GetGameObjects()
    {
        return m_GameObjects;
    }

    void Scene::DestroyGameObject(Shark::Core::GameObject* obj)
    {
        m_ObjectsToDestroy.push_back(obj);
    }

    void Scene::CreateCamera()
    {
        GameObject* cam = new GameObject("Main Camera");
        cam->AddComponent<CameraComponent>(45.0f, 16.0f / 9.0f, 0.1f, 1000.0f);
		cam->AddComponent<CameraController>(5.0f, 0.1f);
		cam->GetTransform().position = { 0.0f, 0.0f, 5.0f };

        AddGameObject(cam);
    }

    void Scene::CreateDirectionalLight()
    {
        GameObject* directionalLight = new GameObject("Directional Light");
        directionalLight->AddComponent<LightComponent>();
        directionalLight->GetComponent<LightComponent>()->Type = LightType::Directional;
        directionalLight->GetComponent<LightComponent>()->Color = { 1.0f, 1.0f, 1.0f };
        directionalLight->GetTransform().position = { 0.0f, 5.0f, 0.0f };
        directionalLight->GetTransform().Rotate({ -45.0f, -45.0f, 0.0f });


        AddGameObject(directionalLight);
    }

    CameraComponent* Scene::GetCamera() const
    {
        for(GameObject* obj : m_GameObjects) {
            auto* cam = obj->GetComponent<CameraComponent>();
            if (cam) {
                return cam;
            }
		}
        return nullptr;
    }

}
