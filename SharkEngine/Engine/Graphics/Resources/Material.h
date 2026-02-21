#ifndef MATERIAL_H
#define MATERIAL_H

#include "Shader.h"
#include "Texture.h"

namespace Shark::Graphics {
	class Material {
	public:

		Material(Shader* shaderProgram = nullptr, Texture* m_Texture = nullptr);
		~Material();

		void Bind() const;

		void SetShader(const std::string& name, const std::string& vertPath, const std::string& fragPath);
		const std::string& GetShaderPath() const { return m_ShaderPath; }
		Shader* GetShader() { return m_Shader; }
		
		void SetTexture(const std::string& path);
		const std::string& GetTexturePath() const { return m_TexturePath; }
		Texture* GetTexture() { return m_Texture; }

	private:
		std::string m_TexturePath;
		std::string m_ShaderPath;

		Texture* m_Texture{ nullptr };
		Shader* m_Shader{ nullptr };

		void CreateDefaultShader();
		void CreateDefaultTexture();
	};
}

#endif