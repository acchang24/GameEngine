#include "Texture.h"
#include <iostream>
#include <glad/glad.h>
#include "stb_image.h"
#include "../MemoryManager/AssetManager.h"

Texture::Texture(const std::string& textureFile) :
	mName(textureFile),
	mTextureID(0),
	mWidth(0),
	mHeight(0),
	mNumChannels(0),
	mType(TextureType::None)
{
	// Create texture object
	glGenTextures(1, &mTextureID);
}

Texture::~Texture()
{
	std::cout << "Deleted texture: \"" << mName << "\"\n";
	glDeleteTextures(1, &mTextureID);
}

void Texture::BindTexture() const
{
	glBindTexture(GL_TEXTURE_2D, mTextureID);
}

void Texture::SetType(TextureType type) 
{ 
	mType = type; 

	LoadTexture();
}

void Texture::LoadTexture()
{
	BindTexture();

	// Load in texture file with stbi_load:
	unsigned char* data = stbi_load(mName.c_str(), &mWidth, &mHeight, &mNumChannels, 4);

	if (data)
	{
		// Get the format based on the number of color channels
		GLenum wrap = 0;
		GLenum dataFormat = GL_RGBA;
		GLenum internalFormat = GL_SRGB_ALPHA;

		if (mType == TextureType::Normal || mType == TextureType::Specular)
		{
			internalFormat = dataFormat;
		}

		// Set the texture's wrapping parameters
		// GL_REPEAT: The default behavior. Repeats the texture image.
		// GL_MIRRORED_REPEAT: Same as GL_REPEAT but mirrors the image with each repeat
		// GL_CLAMP_TO_EDGE: Clamps coordinates between 0 and 1. Higher coordinates become
		// clamped to the edge, resulting in a stretched edge pattern.
		// GL_CLAMP_TO_BORDER: Coordinates outside the range are now given a user specifed vorder color
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		// Set texture filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		//   Start generating a texture using the loaded image data
		//   Textures are generated with glTexImage2D:
		// - 1st argument specifies the texture target. Setting to GL_TEXTURE_2D
		//   will generate textures on the bound texture object at the same target
		// - 2nd argument specifies mipmap level to create a texture for. 0 is base level
		// - 3rd argument specifies the format to store the texture.
		// - 4th/5th arguments specifies width/height of the resulting texture
		// - 6th argument default to 0
		// - 7th/8th arguments specifies the format and datatype of the source image
		//   Loaded the image with RGB values, and stored them as chars(bytes)
		// - Last argument is the actual image data
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, mWidth, mHeight, 0, dataFormat, GL_UNSIGNED_BYTE, data);

		// Automatically generate all the required mipmaps for the currently bound texture
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture\n";
	}

	// Free image data
	stbi_image_free(data);
}
