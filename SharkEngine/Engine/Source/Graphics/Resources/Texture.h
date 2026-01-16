#ifndef TEXTURES_H
#define TEXTURES_H

class Texture {
public:

	/*
	* Creates a new m_Texture by providing a filepath to the target m_Texture
	* @param filePath - The file path to the m_Texture
	*/
	explicit Texture(const char* filePath);
	unsigned int GetID() const {
		return m_ID;
	}

private:

	unsigned int m_ID;
};

#endif // TEXTURES_H
