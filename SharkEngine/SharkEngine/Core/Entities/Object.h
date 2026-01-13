#ifndef OBJECT_H
#define OBJECT_H


#include <string>
#include <vector>
#include <type_traits>

class Object
{
public:

	explicit Object(const std::string& name = "Unamed Object");
	virtual ~Object();

	virtual void Update(float deltaTime) = 0;

	// Utility functions
	const void SetName(const std::string& value) { name = value; }
	const std::string& GetName() const { return name; }
	const std::vector<Object*>& GetSubObjects() const { return SubObjects; }

	template<typename T, typename... Args>
	T* CreateDefaultSubobject(const std::string& name, Args&&... args) {
		static_assert(std::is_base_of<Object, T>::value, "T must be derived from Object");

		T* sub = new T(std::forward<Args>(args)...);
		sub->name = name;
		SubObjects.push_back(sub);
		return sub;
	}

protected:
	std::string name;
	std::vector<Object*> SubObjects; 
};

#endif