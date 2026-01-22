#ifndef COMPONENT_H
#define COMPONENT_H

#include <string>

namespace Shark::Entities {
	class GameObject;
}

namespace Shark::Components {

	class Component
	{
	public:
		explicit Component(Shark::Entities::GameObject* owner, const std::string& name = "Component")
			: owner(owner), name(name) {
		}
		virtual ~Component() = default;

		virtual void Update(float deltaTime) = 0;

		const std::string& GetName() const { return name; }
		Shark::Entities::GameObject* GetOwner() const { return owner; }

	protected:
		std::string name;
		Shark::Entities::GameObject* owner;
	};
}


#endif // COMPONENT_H