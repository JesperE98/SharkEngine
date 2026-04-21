#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Core/Object.h"
#include "Math/Transform.h"
#include "Components/Component.h"
#include <vector>
#include <unordered_map>
#include <typeindex>

namespace Shark::Core {

	class GameObject : public Object
	{
	public:

		bool bMarkedForDeletion = false;

		explicit GameObject(const std::string& name = "GameObject");
		virtual ~GameObject();

#pragma region Engine Logic
		void Tick(float deltaTime);
		void OnDelete();
#pragma endregion


		Math::Transform& GetTransform() { return m_Transform; }
		const Math::Transform& GetTransform() const { return m_Transform; }

		template<typename T, typename... Args>
		T* AddComponent(Args&&... args) {
			static_assert(std::is_base_of<Object, T>::value, "T must be derived from Object");

			T* comp = new T(std::forward<Args>(args)...);

			comp->SetOwner(this);

			m_Components.push_back(comp);

			m_ComponentMap[std::type_index(typeid(T))] = static_cast<T*>(comp);

			return comp;
		}

		template<typename T>
		void RemoveComponent() {
			std::type_index idx(typeid(T));
			m_ComponentMap.erase(idx);

			for (auto it = m_Components.begin(); it != m_Components.end(); ++it){
				if (dynamic_cast<T*>(*it)) {
					delete* it;
					m_Components.erase(it);
					return;
				}
			}
		}

		template<typename T>
		T* GetComponent() {
			auto it = m_ComponentMap.find(std::type_index(typeid(T)));

			if(it != m_ComponentMap.end()){
				return static_cast<T*>(it->second);
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
		Math::Transform m_Transform;

#pragma region Container Member Variables
		std::vector<Components::Component*> m_Components;
		std::unordered_map<std::type_index, void*> m_ComponentMap;
		std::vector<GameObject*> m_Children;
		GameObject* m_Parent{ nullptr };
#pragma endregion
	};

}

#endif // GAMEOBJECT_H