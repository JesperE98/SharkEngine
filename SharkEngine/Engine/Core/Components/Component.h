#ifndef COMPONENT_H
#define COMPONENT_H

#include <string>

class GameObject;

class Component
{
public:
	explicit Component(GameObject* owner, const std::string& name = "Component")
		: owner(owner), name(name) {}
	virtual ~Component() = default;

	virtual void Update(float deltaTime) = 0;

	const std::string& GetName() const { return name; }
	GameObject* GetOwner() const { return owner; }

protected:
	std::string name;
	GameObject* owner;
};

#endif // COMPONENT_H