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

		if (m_Parent) {
			m_Transform.position = m_Parent->GetTransform().position + m_LocalOffsetFromParent;
		}

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
		// Self-parent guard
		if (newParent == this) {
			SE_WARN(Engine, "GameObject '{}' cannot be its own parent", name);
			return;
		}


		// Cycle detection - make sure newParent isn't a descendant
		GameObject* check = newParent;
		while (check) {
			if (check == this) {
				SE_WARN(Engine, "Parenting '{}' to '{}' would create a cycle", name, newParent->name);
				return;
			}
			check = const_cast<GameObject*>( check->GetParent() );
		}

		// Remove from old parent's children list
		if (m_Parent) {
			auto& siblings = m_Parent->m_Children;
			siblings.erase(std::remove(siblings.begin(), siblings.end(), this), siblings.end());
		}

		if (newParent) {
			// Captures relative offset at the moment of parenting
			m_LocalOffsetFromParent = m_Transform.position - newParent->GetTransform().position;
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
