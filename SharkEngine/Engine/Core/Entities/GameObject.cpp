#include "GameObject.h"
#include "Core/Components/Component.h"
#include <string>
#include "Object.h"

GameObject::GameObject(const std::string& name)
	: Object(name)
{
}

GameObject::~GameObject() {
	for (Component* comp : components) {
		delete comp;
		comp = nullptr;
	}

	components.clear();
}

void GameObject::AddComponent(Component* comp) {
	components.push_back(comp);
}

void GameObject::AddChild(GameObject* childObject)
{
	m_Children.push_back(childObject);
	childObject->m_Parent = this;
}

void GameObject::Update(float deltaTime) {
	for (Component* comp : components) {
		comp->Update(deltaTime);
	}
}