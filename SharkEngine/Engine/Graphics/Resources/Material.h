#ifndef MATERIAL_H
#define MATERIAL_H

#include "Shader.h"
#include "Texture.h"
#include "Math/Vector3.h"


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

		void SetSpecularTexture(const std::string& path);
		Texture* GetSpecularTexture() { return m_SpecularTexture; }
		std::string GetSpecularTexturePath() const { return m_SpecularTexturePath; }

		void SetShininess(float value) { m_Shininess = value; }
		float GetShininess() const { return m_Shininess; }

	private:
		bool m_bUseTexture = false;
		Shark::Math::Vector3 m_BaseColor{ 1.0f, 1.0f, 1.0f }; // Default white color if no m_Texture is used

		Texture* m_Texture{ nullptr }; // Diffuse m_Texture
		Texture* m_SpecularTexture{ nullptr }; // Specular m_Texture (optional, can be nullptr)
		Shader* m_Shader{ nullptr };

		float m_Shininess = 32.0f; // Default Phong m_Shininess value

		std::string m_TexturePath;
		std::string m_SpecularTexturePath;
		std::string m_ShaderPath;


		void CreateDefaultShader();
	};
}

#endif