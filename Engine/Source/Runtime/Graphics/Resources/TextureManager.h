#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include "Core/Messaging/MessageSystem.h"
#include <unordered_map>

namespace Shark::Graphics { class Texture; }

namespace Shark::Resources {

	class TextureManager
	{
	public:
		Core::MessageSystem inbox;

		static TextureManager& Get() {
			static TextureManager instance;
			return instance;
		}

		void Update(float DeltaTime);
		void Shutdown();

		Graphics::Texture* LoadTexture(const std::string& filePath);

		void ProcessLoadRequest(const std::string& path);
		void SetResponseTarget(Core::MessageSystem* target);
		
	private:
		std::unordered_map<std::string, Graphics::Texture*> m_TextureCache;
		Core::MessageSystem* m_ResponseTarget = nullptr;

		TextureManager() = default;
		~TextureManager() = default;

		// Delete copy constructor and assignment operator
		TextureManager(const TextureManager&) = delete;
		TextureManager& operator=(const TextureManager&) = delete;
	};
}
#endif // TEXTUREMANAGER_H