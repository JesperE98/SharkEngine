#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Core/Object.h"
#include "Math/Transform.h"
#include "Components/Component.h"
#include <vector>

namespace Shark::Core {

	class GameObject : virtual public Shark::Object
	{
	public:

		bool bMarkedForDeletion = false;

		explicit GameObject(const std::string& name = "GameObject");
		virtual ~GameObject();

#pragma region Engine Logic
		void Tick(float deltaTime);
		void OnDelete();
#pragma endregion


		Shark::Math::Transform& GetTransform() { return m_Transform; }
		const Shark::Math::Transform& GetTransform() const { return m_Transform; }

		template<typename T, typename... Args>
		T* AddComponent(Args&&... args) {
			static_assert(std::is_base_of<Object, T>::value, "T must be derived from Object");

			T* comp = new T(std::forward<Args>(args)...);

			comp->SetOwner(this);

			m_Components.push_back(comp);

			return comp;
		}

		template<typename T>
		void RemoveComponent() {
			for (auto it = m_Components.begin(); it != m_Components.end(); ++it) {
				if (dynamic_cast<T*>(*it)) {
					delete* it; // Free memory
					m_Components.erase(it); // Remove from list
					return;
				}
			}
		}

		template<typename T>
		T* GetComponent() {
			for (auto* comp : m_Components) {
				if (auto* casted = dynamic_cast<T*>(comp)) {
					return casted;
				}
			}
			return nullptr;
		}

#pragma region Hierarchy Management
		void SetParent(GameObject* newParent);
		void AddChild(GameObject* childObject);

		const std::vector<GameObject*>& GetChildren() const { return m_Children; }
		const GameObject* GetParent() const { return m_Parent; }
#pragma endregion
	protected:
		// Core data
		Shark::Math::Transform m_Transform;

#pragma region Container Member Variables
		std::vector<Shark::Components::Component*> m_Components;
		std::vector<GameObject*> m_Children;
		GameObject* m_Parent{ nullptr };
#pragma endregion
	};

}

#endif // GAMEOBJECT_H