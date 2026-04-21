#include "Material.h"
#include "Core/Utilities/Debug.h"
#include "ShaderManager.h"
#include "TextureManager.h"
#include <glad/glad.h>

namespace Shark::Graphics {

	using Resources::ShaderManager;
	using Resources::TextureManager;
	using Math::Vector3;

	Material::Material(Shader* shaderProgram, Texture* texture)
		: m_Shader(shaderProgram), m_diffuseTexture(texture)
	{
		SE_LOG(Resources, "Material::Material() - Creating Material.");

		if (!m_Shader) {
			CreateDefaultShader();
		}

		if (!m_diffuseTexture) {
			m_bUseTexture = false;
			m_BaseColor = Vector3(1.0f, 1.0f, 1.0f);
		}
		else {
			m_bUseTexture = true;
		}

	}

	Material::~Material()
	{
		m_Shader = nullptr; // ShaderManager handles cleanup
		m_diffuseTexture = nullptr; // TextureManager handles cleanup
		m_SpecularTexture = nullptr; // TextureManager handles cleanup
		m_ShaderPath = "";
		m_TexturePath = "";
		m_SpecularTexturePath = "";
	}

	void Material::Bind() const {

		m_Shader->SetInt("uUseTexture", static_cast<int>(m_bUseTexture));
		m_Shader->SetVector3("uBaseColor", m_BaseColor);

		if (m_diffuseTexture) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_diffuseTexture->GetID());
			m_Shader->SetInt("uDiffuseMap", 0);
		}

		bool bUseSpecMap = (m_SpecularTexture != nullptr);
		m_Shader->SetBool("uUseSpecularMap", bUseSpecMap);

		if(bUseSpecMap) {
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, m_SpecularTexture->GetID());
			m_Shader->SetInt("uSpecularMap", 1);
		}

		m_Shader->SetFloat("uShininess", m_Shininess);
	}

	void Material::SetShader(const std::string& name, const std::string& vertPath, const std::string& fragPath)
	{
		Shader* newShader = ShaderManager::Get().LoadShader(name, vertPath, fragPath);

		if (newShader) {
			m_Shader = newShader;
			m_ShaderPath = name;
			SE_SUCC(Resources, "Material::SetShader() - Shader set to: {}", name);
		}
	}

	void Material::SetTexture(const std::string& path)
	{
		Texture* newTex = TextureManager::Get().LoadTexture(path);

		if (newTex) {
			m_diffuseTexture = newTex;
			m_TexturePath = path;
			m_bUseTexture = true;
		}
		else {
			m_bUseTexture = false;
		}
	}

	void Material::SetSpecularTexture(const std::string& path)
	{
		if (path.empty()) {
			m_SpecularTexture = nullptr;
			m_SpecularTexturePath = "";
			return;
		}

		Texture* newSpecTex = TextureManager::Get().LoadTexture(path);
		if (newSpecTex) {
			m_SpecularTexture = newSpecTex;
			m_SpecularTexturePath = path;
		}
	}

	void Material::CreateDefaultShader() {
		SetShader("SE_BasicLit", "SE_BasicLit.vert.glsl", "SE_BasicLit.frag.glsl");
	}
}
