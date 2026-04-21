#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H

#include "Core/Messaging/MessageSystem.h"

#include <unordered_map>

namespace Shark::Graphics { class Shader; }

namespace Shark::Resources {
	class ShaderManager
	{
	public:
		Core::MessageSystem inbox;
		
		static ShaderManager& Get();

		void Update(float DeltaTime);
		void Shutdown();
		void ProcessLoadRequest(const std::string& path);
		void SetResponseTarget(Shark::Core::MessageSystem* target);

		// Return an exisiting shader or loads/compiles a new one
		Graphics::Shader* LoadShader(const std::string& name, const std::string& vertPath, const std::string& fragPath);
		Graphics::Shader* LoadShader(
			const std::string& name,
			const std::string& vertPath,
			const std::string& fragPath,
			const std::string& geomPath
		);
		// Convenience: find by name if you know a shader is already loaded
		const Graphics::Shader* GetShader(const std::string& name) const;

	private:
		std::unordered_map<std::string, Graphics::Shader*> m_ShaderCache;
		Core::MessageSystem* m_ResponseTarget = nullptr;

		ShaderManager() = default;
		~ShaderManager() = default;

		// Delete copy constructor and assignment operator
		ShaderManager(const ShaderManager&) = delete;
		ShaderManager& operator=(const ShaderManager&) = delete;
	};
}
#endif // SHADERMANAGER_H