#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H

#include "Core/Messaging/MessageQueue.h"

#include <unordered_map>

namespace Shark::Graphics { class Shader; }

namespace Shark::Managers {
	class ShaderManager
	{
	public:
		Shark::Core::MessageQueue inbox;
		
		static ShaderManager& Get();

		void Update(float DeltaTime);
		void Shutdown();
		void ProcessLoadRequest(const std::string& path);

		// Return an exisiting shader or loads/compiles a new one
		Shark::Graphics::Shader* LoadShader(const std::string& name, const std::string& vertPath, const std::string& fragPath);

		// Convenience: find by name if you know a shader is already loaded
		const Shark::Graphics::Shader* GetShader(const std::string& name) const;

	private:
		std::unordered_map<std::string, Shark::Graphics::Shader*> m_ShaderCache;

		ShaderManager() = default;
		~ShaderManager() = default;

		// Delete copy constructor and assignment operator
		ShaderManager(const ShaderManager&) = delete;
		ShaderManager& operator=(const ShaderManager&) = delete;
	};
}
#endif // SHADERMANAGER_H