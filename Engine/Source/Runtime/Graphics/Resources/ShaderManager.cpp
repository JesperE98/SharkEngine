#include "ShaderManager.h"
#include "Core/Utilities/Debug.h"
#include "Graphics/Resources/Shader.h"

namespace Shark::Resources {

    using Graphics::Shader;
    using Core::Message;
	using Core::EventType;
    using Core::MessageSystem;

    ShaderManager& ShaderManager::ShaderManager::Get()
    {
		static ShaderManager instance;
        return instance;
    }

    void ShaderManager::Update(float DeltaTime)
    {
        Message msg;
        while (inbox.Pop(msg)) {
            if (msg.type == EventType::LoadShader) {
                ProcessLoadRequest(msg.payload);
            }
        }
    }

    void ShaderManager::Shutdown()
    {
        for (auto& pair : m_ShaderCache) {
            if (pair.second) {
                delete pair.second;
            }
        }

        m_ShaderCache.clear();
    }

    void ShaderManager::ProcessLoadRequest(const std::string& path)
    {
		SE_PROC(Resources, "ShaderManager::ProcessLoadRequest() - Handshake sent for shader: {}", path);

		Shader* loadedShader = LoadShader(path, path + ".vert.glsl", path + ".frag.glsl");

        if (loadedShader) {
            Message reply;
            reply.type = EventType::ShaderLoaded;
            reply.payload = path;
            reply.data = static_cast<Shader*>(loadedShader);

            // Send back to the Editor Manager
            if (m_ResponseTarget) m_ResponseTarget->Push(reply);

        }
        else {
            Message errorMsg;
            errorMsg.type = EventType::ErrorMessage;
            errorMsg.payload = "Failed to load shader at: " + path;
            
            if (m_ResponseTarget) m_ResponseTarget->Push(errorMsg);

            SE_ERR(Resources, "ShaderManager::ProcessLoadRequest() - Failed to load shader at: {}", path);
        }
    }

    void ShaderManager::SetResponseTarget(MessageSystem* target)
    {
        m_ResponseTarget = target;
    }

    Shader* ShaderManager::LoadShader(const std::string& name, const std::string& vertPath, const std::string& fragPath)
    {
		if (m_ShaderCache.find(name) != m_ShaderCache.end()) {
            return m_ShaderCache[name];
        }

		Shader* newShader = new Shader(vertPath.c_str(), fragPath.c_str());
		m_ShaderCache[name] = newShader;

		SE_LOG(Resources, "ShaderManager::LoadShader() - Compiled and Cached shader: {} (vert: {}, frag: {})", name, vertPath, fragPath);
        return newShader;
    }

    Shader* ShaderManager::LoadShader(const std::string& name, const std::string& vertPath, const std::string& fragPath, const std::string& geomPath)
    {
        if (m_ShaderCache.find(name) != m_ShaderCache.end()) {
            return m_ShaderCache[name];
        }

        auto* newShader = new Shader(vertPath.c_str(), fragPath.c_str(), geomPath.c_str());
        m_ShaderCache[name] = newShader;

        SE_LOG(Resources, "ShaderManager::LoadShader() - Compiled and Cached shader: {} (vert: {}, frag: {}, geom: {})", name, vertPath, fragPath, geomPath);
        return newShader;
    }

    const Shader* ShaderManager::GetShader(const std::string& name) const
    {
        if(m_ShaderCache.find(name) != m_ShaderCache.end()) {
			SE_SUCC(Resources, "ShaderManager::GetShader() - Cache hit for shader: {}", name);
            return m_ShaderCache.at(name);
		}

		SE_WARN(Resources, "ShaderManager::GetShader() - Cache miss for shader: {}", name);
        return nullptr;
    }
}