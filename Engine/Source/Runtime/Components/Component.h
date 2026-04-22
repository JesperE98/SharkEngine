#ifndef COMPONENT_H
#define COMPONENT_H

#include "Core/Object.h"

namespace Shark::Core {
	class GameObject;
}

namespace Shark::Components {

	enum class TickMode : uint8_t {
		PlayOnly,
		EditOnly,
		Always
	};

	class Component : virtual public Object
	{
	public:

		bool bEnabled = true;
		TickMode tickMode = TickMode::PlayOnly;

		Component() : m_Owner(nullptr) {}

		explicit Component(Core::GameObject* owner)	: m_Owner(owner) {}
		virtual ~Component() = default;

		virtual void Update(float deltaTime) {};

		void SetOwner(Core::GameObject* owner) { m_Owner = owner; }
		Core::GameObject* GetOwner() const { return m_Owner; }

	protected:

		Core::GameObject* m_Owner;
	};
}


#endif // COMPONENT_H