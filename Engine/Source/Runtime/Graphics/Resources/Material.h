#ifndef MATERIAL_H
#define MATERIAL_H

#include "Shader.h"
#include "Texture.h"
#include "Math/Vector3.h"


namespace Shark::Graphics {
	class Material {
	public:


		Material(Shader* shaderProgram = nullptr, Texture* m_diffuseTexture = nullptr);
		~Material();

		void Bind() const;

		void SetShader(const std::string& name, const std::string& vertPath, const std::string& fragPath);
		const std::string& GetShaderPath() const { return m_ShaderPath; }
		Shader* GetShader() { return m_Shader; }
		
		void SetTexture(const std::string& path);
		const std::string& GetTexturePath() const { return m_TexturePath; }
		Texture* GetTexture() { return m_diffuseTexture; }

		void SetSpecularTexture(const std::string& path);
		Texture* GetSpecularTexture() { return m_SpecularTexture; }
		std::string GetSpecularTexturePath() const { return m_SpecularTexturePath; }

		void SetShininess(float value) { m_Shininess = value; }
		float GetShininess() const { return m_Shininess; }

		bool GetUseMipMaps() const { return bUseMipMaps; }
		void SetUseMipMaps(bool value) { 
			bUseMipMaps = value;

			if (m_diffuseTexture) {
				m_diffuseTexture->UpdateFiltering(value);
			}
		};

	private:
		bool m_bUseTexture = false;
		Shark::Math::Vector3 m_BaseColor = { 1.0f, 1.0f, 1.0f };; // Default white color if no m_diffuseTexture is used

		Texture* m_diffuseTexture{ nullptr }; // Diffuse m_diffuseTexture
		Texture* m_SpecularTexture{ nullptr }; // Specular m_diffuseTexture (optional, can be nullptr)
		Shader* m_Shader{ nullptr };

		float m_Shininess = 32.0f; // Default Phong m_Shininess value

		std::string m_TexturePath;
		std::string m_SpecularTexturePath;
		std::string m_ShaderPath;

		bool bUseMipMaps = true;


		void CreateDefaultShader();
	};
}

#endif