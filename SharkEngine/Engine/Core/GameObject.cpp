#include "GameObject.h"
#include "Components/Component.h"

namespace Shark::Core {

	using Shark::Components::Component;

	GameObject::GameObject(const std::string& name) {
		this->name = name;
	}

	GameObject::~GameObject() {
		OnDelete();
	}

	void GameObject::Tick(float deltaTime) {
		for (Component* comp : m_Components) {
			if (comp->bEnabled) {
				comp->Update(deltaTime);
			}
		}

		for (auto* child : m_Children) {
			child->Tick(deltaTime);
		}

		if (bMarkedForDeletion == true) {
			OnDelete();
		}
	}

	void GameObject::OnDelete()
	{
		for (Component* comp : m_Components) {
			delete comp;
		}
		m_Components.clear();

		for (auto* child : m_Children) {
			child->OnDelete();
		}
	}

	void GameObject::SetParent(GameObject* newParent) {

		if (m_Parent) {

		}

		m_Parent = newParent;

		if (m_Parent) {
			m_Parent->m_Children.push_back(this);
		}
	}

	void GameObject::AddChild(GameObject* childObject)
	{
		if (!childObject) {
			SE_WARN(Temp ,"GameObject::AddChild() - Attempted to add a null child to GameObject '%s'", name.c_str());
			return;
		}

		childObject->SetParent(this);
	}

}
