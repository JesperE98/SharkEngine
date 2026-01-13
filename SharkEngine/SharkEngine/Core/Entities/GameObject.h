#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Object.h"
#include "Core/Components/Transform.h"
#include <string>
#include <vector>

class Component;

class GameObject :	public Object
{
public:
	explicit GameObject(const std::string& name = "GameObject");
	virtual ~GameObject();

	void Update(float deltaTime) override;

	Transform& GetTransform() { return transform; }
	const Transform& GetTransform() const { return transform; }

	void AddComponent(Component* comp);
	void AddChild(GameObject* childObject);
	
	template<typename T>
	T* GetComponent() {
		for (auto* comp : components) {
			if (auto* casted = dynamic_cast<T*>(comp)) {
				return casted;
			}
		}
		return nullptr;
	}

	const std::vector<GameObject*>& GetChildren() const { return m_Children; }

protected:
	Transform transform;
	std::vector<Component*> components;
	GameObject* m_Parent{ nullptr };
	std::vector<GameObject*> m_Children;
};

#endif // GAMEOBJECT_H