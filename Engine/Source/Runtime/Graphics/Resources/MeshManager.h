#ifndef MESHMANAGER_H
#define MESHMANAGER_H

#include "Core/Messaging/MessageSystem.h"

#include <unordered_map>
#include <string>

namespace Shark::Core {
	class GameObject;
}

namespace Shark::Graphics {
	class Material;
	class Mesh;
	enum class PrimitiveType;
}

namespace Shark::Resources {

	class MeshManager {
	public:
		Core::MessageSystem inbox;

		static MeshManager& Get() {
			static MeshManager instance;
			return instance;
		}
		
		void Update(float DeltaTime);
		void AddMeshToCache(const std::string& key, Graphics::Mesh* mesh);
		void SetResponseTarget(Core::MessageSystem* target);

		Graphics::Mesh* GetMesh(const std::string& filePath);
		Graphics::Mesh* LoadMesh(Graphics::PrimitiveType type);
		Graphics::Mesh* LoadMesh(const std::string& filePath);
		Graphics::Mesh* LoadMeshSync(const std::string& path);

		void Shutdown();
		static void CreateFromObj(Core::GameObject* obj, const std::string& filePath, Graphics::Material* mat = nullptr);

	private:

		std::unordered_map<std::string, Graphics::Mesh*> m_MeshCache;
		Core::MessageSystem* m_ResponseTarget = nullptr;

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
		void ProcessLoadRequest<Graphics::PrimitiveType>(const Graphics::PrimitiveType& type);

	};
}

#endif