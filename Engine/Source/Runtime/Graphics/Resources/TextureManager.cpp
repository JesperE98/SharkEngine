#include "TextureManager.h"
#include "Texture.h"
#include "Core/Utilities/Debug.h"

namespace Shark::Resources {

    using Core::Message;
	using Core::EventType;
    using Core::MessageSystem;
    using Graphics::Texture;

    void TextureManager::Update(float DeltaTime)
    {
        Message msg;
        while (inbox.Pop(msg)) {
            if (msg.type == EventType::LoadTexture) {
				ProcessLoadRequest(msg.payload);
            }
        }
    }

    void TextureManager::Shutdown()
    {
        SE_LOG(Resources, "TextureManager::Shutdown() - Cleaning up {} textures.", m_TextureCache.size());
        
        for (auto& pair : m_TextureCache) {
            if (pair.second) {
                delete pair.second;
            }
        }

        m_TextureCache.clear();
    }

    Texture* TextureManager::LoadTexture(const std::string& filePath)
    {
		// 1. Check cache
		auto it = m_TextureCache.find(filePath);
		if (it != m_TextureCache.end()) {
            return it->second;
        }

		SE_WARN(Resources, "TextureManager::LoadTexture() - Cache miss! Loading {} from disk...", filePath);

		// 2. Load from disk
		Texture* newTexture = new Texture(filePath.c_str(), true);

        if (newTexture) {
			m_TextureCache[filePath] = newTexture;
            return newTexture;
        }
        
        delete newTexture;
        return nullptr;
    }

    void TextureManager::ProcessLoadRequest(const std::string& path)
    {
        SE_PROC(Resources, "TextureManager::ProcessLoadRequest() - Handshake sent for texture: {}", path);
        Texture* loadedTexture = LoadTexture(path);

        if (loadedTexture) {
            Message reply;
            reply.type = EventType::TextureLoaded;
            reply.payload = path;
            reply.data = static_cast<void*>(loadedTexture);

            // Send back to the Editor Manager
            if (m_ResponseTarget) m_ResponseTarget->Push(reply);

        }
        else {
            Message errorMsg;
            errorMsg.type = EventType::ErrorMessage;
            errorMsg.payload = "Failed to load texture at: " + path;
            
            if (m_ResponseTarget) m_ResponseTarget->Push(errorMsg);

            SE_ERR(Resources, "TextureManager::ProcessLoadRequest() - Failed to load texture at: {}", path);
        }
    }

    void TextureManager::SetResponseTarget(MessageSystem* target)
    {
        m_ResponseTarget = target;
    }
}

