#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

class Scene;

class SceneManager
{
public:

	static SceneManager& Get();

	void SetActiveScene(Scene* scene);
	void Update(float deltaTime);

	Scene* GetActiveScene() const;

private:
	SceneManager() = default;
	~SceneManager();

	// Disable copying
	SceneManager(const SceneManager&) = delete;	
	SceneManager& operator=(const SceneManager&) = delete;

	Scene* m_ActiveScene{ nullptr };
};

#endif // SCENE_MANAGER_H