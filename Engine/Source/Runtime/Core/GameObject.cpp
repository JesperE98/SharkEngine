#include "GameObject.h"
#include "App/EditorStateManager.h"

namespace Shark::Core {

	using Components::Component;
	using Components::TickMode;
	using Core::EditorStateManager;

	GameObject::GameObject(const std::string& name) {
		this->name = name;
	}

	GameObject::~GameObject() {
		OnDelete();
	}

	void GameObject::Tick(float deltaTime) {
		bool isPlaying = EditorStateManager::Get().IsPlaying();

		for (Component* comp : m_Components) {
			if (!comp->bEnabled) continue;
			
			bool shouldTick = comp->tickMode == TickMode::Always ||
				(comp->tickMode == TickMode::PlayOnly && isPlaying) ||
				(comp->tickMode == TickMode::EditOnly && !isPlaying);

			if (!shouldTick) continue;

			comp->Update(deltaTime);
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
			SE_WARN(Temp ,"Attempted to add a null child to GameObject '%s'", name.c_str());
			return;
		}

		childObject->SetParent(this);
	}

}
