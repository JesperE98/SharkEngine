#ifndef COMPONENT_H
#define COMPONENT_H

#include "Core/Object.h"

namespace Shark::Core {
	class GameObject;
}

namespace Shark::Components {

	class Component : virtual public Shark::Object
	{
	public:

		bool bEnabled = true;

		Component() : m_Owner(nullptr) {}

		explicit Component(Shark::Core::GameObject* owner)	: m_Owner(owner) {}
		virtual ~Component() = default;

		virtual void Update(float deltaTime) {};

		void SetOwner(Shark::Core::GameObject* owner) { m_Owner = owner; }
		Shark::Core::GameObject* GetOwner() const { return m_Owner; }

	protected:

		Shark::Core::GameObject* m_Owner;
	};
}


#endif // COMPONENT_H