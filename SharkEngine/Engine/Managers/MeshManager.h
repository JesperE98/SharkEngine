#ifndef MESHMANAGER_H
#define MESHMANAGER_H

#include "Core/Messaging/MessageQueue.h"

#include <unordered_map>
#include <string>

namespace Shark::Core {
	class GameObject;
}

namespace Shark::Graphics {
	enum class PrimitiveType;
	class Material;
	class Mesh;
}

namespace Shark::Managers {

	class MeshManager {
	public:
		Shark::Core::MessageQueue inbox;

		static MeshManager& Get() {
			static MeshManager instance;
			return instance;
		}
		void Update(float DeltaTime);
		void AddMeshToCache(const std::string& key, Shark::Graphics::Mesh* mesh);

		Shark::Graphics::Mesh* GetMesh(const std::string& filePath);
		Shark::Graphics::Mesh* LoadMesh(Shark::Graphics::PrimitiveType type);
		Shark::Graphics::Mesh* LoadMesh(const std::string& filePath);

		void Shutdown();
		static void CreateFromObj(Shark::Core::GameObject* obj, const std::string& filePath, Shark::Graphics::Material* mat = nullptr);

	private:

		std::unordered_map<std::string, Shark::Graphics::Mesh*> m_MeshCache;

		MeshManager() = default;
		~MeshManager() = default;

		// Delete copy constructor and assignment operator
		MeshManager(const MeshManager&) = delete;
		MeshManager& operator=(const MeshManager&) = delete;

		template<typename T>
		void ProcessLoadRequest(const T& value);

		template<>
		void ProcessLoadRequest<std::string>(const std::string& path);
		template<>
		void ProcessLoadRequest<Shark::Graphics::PrimitiveType>(const Shark::Graphics::PrimitiveType& type);

	};
}

#endif