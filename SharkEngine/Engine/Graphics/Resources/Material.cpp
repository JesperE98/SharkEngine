#include "Material.h"
#include "Core/Utilities/Debug.h"
#include "Managers/ShaderManager.h"
#include "Managers/TextureManager.h"
#include <glad/glad.h>

namespace Shark::Graphics {

	using Shark::Managers::ShaderManager;
	using Shark::Managers::TextureManager;

	Material::Material(Shader* shaderProgram, Texture* texture)
		: m_Shader(shaderProgram), m_Texture(texture)
	{
		SE_LOG(Resources, "Material::Material() - Creating Material.");

		if (!m_Shader) {
			CreateDefaultShader();
		}

		if (!m_Texture) {
			CreateDefaultTexture();
		}

	}

	Material::~Material()
	{
		m_Shader = nullptr; // ShaderManager handles cleanup
		m_Texture = nullptr; // TextureManager handles cleanup
		m_ShaderPath = "";
		m_TexturePath = "";
	}

	void Material::Bind() const {

		if (m_Texture) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_Texture->GetID());
			m_Shader->SetInt("u_Texture", 0);
		}
	}

	void Material::SetShader(const std::string& name, const std::string& vertPath, const std::string& fragPath)
	{
		Shader* newShader = Managers::ShaderManager::Get().LoadShader(name, vertPath, fragPath);

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
			m_Texture = newTex;
			m_TexturePath = path;
		}
	}

	void Material::CreateDefaultShader() {
		SetShader("SE_BasicLit", "Shaders/SE_BasicLit.vert.glsl", "Shaders/SE_BasicLit.frag.glsl");
	}

	void Material::CreateDefaultTexture() {
		SetTexture("Textures/box_wood.jpg");
	}
}
