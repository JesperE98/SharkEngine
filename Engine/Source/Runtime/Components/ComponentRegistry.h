#ifndef COMPONENT_REGISTRY_H
#define COMPONENT_REGISTRY_H

#include <Core/GameObject.h>

#include <functional>
#include <string>
#include <unordered_map>
#include <vector>
#include <type_traits>

namespace Shark::Components {

	class ComponentRegistry {
	public:
		using FactoryFn = std::function < Component* ( Core::GameObject* )>;

		static ComponentRegistry& Get() {
			static ComponentRegistry instance;
			return instance;
		}

		template<typename T>
		void Register(const std::string& name) {
			static_assert( std::is_base_of_v<Component, T>, "T must derive from Shark::Components::Component" );

			m_Factories[name] = [](Core::GameObject* obj) -> Component* {
				return obj->AddComponent<T>();
			};
		}

		Component* Create(const std::string& name, Core::GameObject* obj) {
			auto it = m_Factories.find(name);
			if (it == m_Factories.end()) return nullptr;
			return it->second(obj);
		}

		std::vector<std::string> GetAllNames() const {
			std::vector<std::string> names;

			for (auto& [name, _] : m_Factories) {
				names.push_back(name);
			}

			return names;
		}

	private:
		std::unordered_map<std::string, FactoryFn> m_Factories;

		ComponentRegistry() = default;
		~ComponentRegistry() = default;

		ComponentRegistry(const ComponentRegistry&) = delete;
		ComponentRegistry& operator=(const ComponentRegistry&) = delete;
	};
}

#endif // COMPONENT_REGISTRY_H