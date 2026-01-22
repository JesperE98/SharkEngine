#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Core/Object.h"
#include "Math/Transform.h"

namespace Shark::Components {
	class Component;
}

namespace Shark::Math {
	struct Transform;
}

namespace Shark::Entities {

	class GameObject : public Shark::Object
	{
	public:
		explicit GameObject(const std::string& name = "GameObject");
		virtual ~GameObject();

		void Update(float deltaTime) override;

		Shark::Math::Transform& GetTransform() { return transform; }
		const Shark::Math::Transform& GetTransform() const { return transform; }

		void AddComponent(Shark::Components::Component* comp);
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

		Shark::Math::Transform transform;
		std::vector<Shark::Components::Component*> components;
		GameObject* m_Parent{ nullptr };
		std::vector<GameObject*> m_Children;
	};

}

#endif // GAMEOBJECT_H