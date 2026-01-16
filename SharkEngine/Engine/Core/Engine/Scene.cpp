#include "Scene.h"
#include "Core/Input/CameraController.h"

Scene::~Scene()
{
    if (m_CameraController) {
        delete m_CameraController;
        m_CameraController = nullptr;
    }

    for (auto& obj : gameObjects) {
        delete obj;
        obj = nullptr;
    }

    gameObjects.clear();
}

void Scene::AddGameObject(GameObject* obj)
{
    gameObjects.push_back(obj);
}

std::vector<GameObject*>& Scene::GetGameObjects()
{
    return gameObjects;
}

void Scene::Update(float deltaTime) {

    for (GameObject* obj : gameObjects) {
        obj->Update(deltaTime);
    }

    if (m_CameraController) {
        m_CameraController->Update(deltaTime);
    }
    // Or alternatively, render all IRenderables
    //for (auto& renderable : m_Renderables) {
    //    renderable->Draw();
    //}
}

void Scene::CreateCamera(const float aspectRatio)
{
    Camera* cam = new Camera("MainCamera", 90.0f, aspectRatio, 0.1f, 100.0f);
    m_Camera = cam;
    m_CameraController = new CameraController(cam, 5.0f, 0.1f);
    AddGameObject(cam);
}
