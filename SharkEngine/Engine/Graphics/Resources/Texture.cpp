#include "Texture.h"
#include "Core/Utilities/Debug.h"
#include "Managers/PathManager.h"

#include <glad/glad.h>
#include <stb/image.h>

namespace Shark::Graphics {

	using Shark::Managers::PathManager;

	Texture::Texture(const char* filePath) {

		int width, height, nrChannels;
		std::string texturePath = PathManager::Get().GetContentPath(filePath);
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
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		GLenum format;
		switch (nrChannels) {
		case 1: format = GL_RED; break;
		case 3: format = GL_RGB; break;
		case 4: // Meaning it's a m_Texture file with an alpha value
			format = GL_RGBA;
			break;
		default: format = GL_RGB; break;
		}

		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		stbi_image_free(data);
	}

	Texture::~Texture()
	{
		if(m_ID != 0){
			glDeleteTextures(1, &m_ID);
			SE_LOG(Resources, "Texture::~Texture() - Deleted texture with ID: {}", m_ID);
		}
	}

}