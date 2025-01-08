#include "CubeMap.h"
#include <iostream>
#include <glad/glad.h>
#include "stb_image.h"
#include "Texture.h"
#include "Shader.h"

CubeMap::CubeMap(const std::vector<std::string>& textureFaces) :
	mTextureID(0),
	mTextureUnit(static_cast<int>(TextureUnit::CubeMap))
{
	// Generate textures and save that id
	glGenTextures(1, &mTextureID);
	// Bind to a open gl cube map
	glBindTexture(GL_TEXTURE_CUBE_MAP, mTextureID);

	int width = 0;
	int height = 0;
	int numChannels = 0;

	// Specify filtering and wrapping methods for cube maps
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	// Set flip vertically to false for cube maps
	stbi_set_flip_vertically_on_load(false);

	// Loop through each faceName and generate a opengl texture
	for (size_t i = 0; i < textureFaces.size(); ++i)
	{
		unsigned char* data = stbi_load(textureFaces[i].c_str(), &width, &height, &numChannels, 4);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load: " << textureFaces[i] << std::endl;
		}

		// Free image data
		stbi_image_free(data);
	}

	// Go back to flipping vertically on y axis (for other textures)
	stbi_set_flip_vertically_on_load(true);
}

CubeMap::~CubeMap()
{
	std::cout << "Delete cube map" << std::endl;
	glDeleteTextures(1, &mTextureID);
}

void CubeMap::SetActive(Shader* s)
{
	glActiveTexture(GL_TEXTURE0 + mTextureUnit);
	s->SetInt("cubeMap", mTextureUnit);
	glBindTexture(GL_TEXTURE_CUBE_MAP, mTextureID);
	glActiveTexture(GL_TEXTURE0);
}
