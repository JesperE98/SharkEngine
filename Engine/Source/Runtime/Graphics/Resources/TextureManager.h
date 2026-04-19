#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include "Core/Messaging/MessageSystem.h"
#include <unordered_map>

namespace Shark::Graphics { class Texture; }

namespace Shark::Managers {
	class TextureManager
	{
	public:
		Shark::Core::MessageSystem inbox;

		static TextureManager& Get() {
			static TextureManager instance;
			return instance;
		}

		void Update(float DeltaTime);
		void Shutdown();

		Shark::Graphics::Texture* LoadTexture(const std::string& filePath);

		void ProcessLoadRequest(const std::string& path);
		void SetResponseTarget(Shark::Core::MessageSystem* target);
		
	private:
		std::unordered_map<std::string, Shark::Graphics::Texture*> m_TextureCache;
		Shark::Core::MessageSystem* m_ResponseTarget = nullptr;

		TextureManager() = default;
		~TextureManager() = default;

		// Delete copy constructor and assignment operator
		TextureManager(const TextureManager&) = delete;
		TextureManager& operator=(const TextureManager&) = delete;
	};
}
#endif // TEXTUREMANAGER_H