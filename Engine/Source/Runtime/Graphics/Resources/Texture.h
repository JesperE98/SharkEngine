#ifndef TEXTURES_H
#define TEXTURES_H

namespace Shark::Graphics {
	class Texture {
	public:

		/*
		* Creates a new m_diffuseTexture by providing a filepath to the target m_diffuseTexture
		* @param filePath - The file path to the m_diffuseTexture
		*/
		explicit Texture(const char* filePath, bool bUseMipMaps);
		~Texture();
		unsigned int GetID() const {
			return m_ID;
		}

		void UpdateFiltering(bool bUseMipMaps);

	private:

		unsigned int m_ID;

		// Helper function for checking Color Channel Format
		int CheckColorChannelFormat(int channels);
	};
}

#endif // TEXTURES_H
