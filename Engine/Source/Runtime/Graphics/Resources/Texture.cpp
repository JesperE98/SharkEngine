#include "Texture.h"
#include "Core/Utilities/Debug.h"
#include "IO/PathManager.h"

#include <glad/glad.h>
#include <stb/image.h>

namespace Shark::Graphics {

	using IO::PathManager;
	using IO::PathCategory;

	Texture::Texture(const char* filePath, bool bUseMipMaps) {

		int width, height, nrChannels;
		std::string texturePath = PathManager::Get().GetPath(PathCategory::Content, filePath);
		stbi_set_flip_vertically_on_load(true); // flips the images y-axis
		unsigned char* data = stbi_load(texturePath.c_str(), &width, &height, &nrChannels, 0);

		if (!data) {
			SE_ERR(Resources, "Texture::Texture() - Failed to load texture at path: {}", texturePath);
			return;
		}

		glGenTextures(1, &m_ID);
		glBindTexture(GL_TEXTURE_2D, m_ID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		if (bUseMipMaps) {
			// Use Trilinear filtering if MipMaps are enabled
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		}
		else { // Use standard Bilinear filtering if MipMaps are disabled
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		}

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		GLenum format = CheckColorChannelFormat(nrChannels);

		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

		if (bUseMipMaps) {
			glGenerateMipmap(GL_TEXTURE_2D);
			SE_LOG(Resources, "MipMaps generated for: {}", filePath);
		}

		stbi_image_free(data);
	}

	Texture::~Texture()
	{
		if(m_ID != 0){
			glDeleteTextures(1, &m_ID);
			SE_LOG(Resources, "Texture::~Texture() - Deleted texture with ID: {}", m_ID);
		}
	}

	void Texture::UpdateFiltering(bool bUseMipMaps)
	{
		glBindTexture(GL_TEXTURE_2D, m_ID);

		if (bUseMipMaps) {
			// Use Trilinear filtering if MipMaps are enabled
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else { // Use standard Bilinear filtering if MipMaps are disabled
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		}
	}

	int Texture::CheckColorChannelFormat(int channels)
	{
		GLenum format;
		switch (channels) {
		case 1: format = GL_RED; break;
		case 3: format = GL_RGB; break;
		case 4: // Meaning it's a m_diffuseTexture file with an alpha value
			format = GL_RGBA;
			break;
		default: format = GL_RGB; break;
		}

		return format;
	}

}