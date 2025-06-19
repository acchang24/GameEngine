#include "Texture.h"
#include <iostream>
#include "../Util/LoggerMacros.h"
#include "stb_image.h"

Texture::Texture(TextureType type) :
	mName(),
	mTextureID(0),
	mWidth(0),
	mHeight(0),
	mNumChannels(0),
	mTextureUnit(static_cast<int>(type)),
	mType(type)
{
	// Create texture object
	glGenTextures(1, &mTextureID);

	mName = "texture_" + std::to_string(mTextureID);
}

Texture::Texture(const std::string& textureFile, TextureType type) :
	mName(textureFile),
	mTextureID(0),
	mWidth(0),
	mHeight(0),
	mNumChannels(0),
	mTextureUnit(static_cast<int>(type)),
	mType(type)
{
	// Create texture object
	glGenTextures(1, &mTextureID);

	LoadTexture();
}

Texture::~Texture()
{
	std::cout << "Deleted texture: \"" << mName << "\"\n";
	glDeleteTextures(1, &mTextureID);
}

void Texture::GenerateTexture(GLenum target, int width, int height, GLenum dataType, const void* data, 
	bool generatesMipMap, bool flipTexture, int numChannels, GLenum wrapS, GLenum wrapT, GLenum minFilter, GLenum maxFilter)
{
	BindTexture();

	// Re-adjust width height and num channels if they haven't been loaded
	if (mWidth == 0 && mHeight == 0 && mNumChannels == 0)
	{
		mWidth = width;
		mHeight = height;
		mNumChannels = numChannels;
	}

	// Set the texture's wrapping parameters
	// GL_REPEAT: The default behavior. Repeats the texture image.
	// GL_MIRRORED_REPEAT: Same as GL_REPEAT but mirrors the image with each repeat
	// GL_CLAMP_TO_EDGE: Clamps coordinates between 0 and 1. Higher coordinates become
	// clamped to the edge, resulting in a stretched edge pattern.
	// GL_CLAMP_TO_BORDER: Coordinates outside the range are now given a user specifed vorder color
	glTexParameteri(target, GL_TEXTURE_WRAP_S, wrapS);
	glTexParameteri(target, GL_TEXTURE_WRAP_T, wrapT);
	// Add wrap r for cube maps and point shadow maps
	if (mType == TextureType::CubeMap || mType == TextureType::PointShadow)
	{
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, wrapS);
	}

	// Set texture filtering parameters
	glTexParameteri(target, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri(target, GL_TEXTURE_MAG_FILTER, maxFilter);

	// Flip image
	stbi_set_flip_vertically_on_load(flipTexture);

	// Setup swizzle mapping
	GLint swizzleMask[4] = { GL_RED, GL_GREEN, GL_BLUE, GL_ALPHA };

	// Configure data format and internal format based on number of channels
	GLenum dataFormat = 0;
	GLenum internalFormat = 0;

	switch (numChannels)
	{
	case 1:
		dataFormat = GL_RED;
		internalFormat = GL_RED;
		swizzleMask[0] = GL_RED;
		swizzleMask[1] = GL_RED;
		swizzleMask[2] = GL_RED;
		swizzleMask[3] = GL_ONE;
		// Add swizzling mask for 1 and 2 channel textures (treat both as RGB)
		glTexParameteriv(target, GL_TEXTURE_SWIZZLE_RGBA, swizzleMask);
		break;
	case 2:
		dataFormat = GL_RG;
		internalFormat = GL_RG;
		swizzleMask[0] = GL_RED;
		swizzleMask[1] = GL_GREEN;
		swizzleMask[2] = GL_ZERO;
		swizzleMask[3] = GL_ONE;
		// Add swizzling mask for 1 and 2 channel textures (treat both as RGB)
		glTexParameteriv(target, GL_TEXTURE_SWIZZLE_RGBA, swizzleMask);
		break;
	case 3:
		dataFormat = GL_RGB;
		if (mType == TextureType::Normal || mType == TextureType::Specular)
		{
			internalFormat = GL_RGB;
		}
		else
		{
			internalFormat = GL_SRGB;
		}
		break;
	case 4:
		dataFormat = GL_RGBA;
		if (mType == TextureType::Normal || mType == TextureType::Specular)
		{
			internalFormat = GL_RGBA;
		}
		else
		{
			internalFormat = GL_SRGB_ALPHA;
		}
		break;
	}

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
	glTexImage2D(target, 0, internalFormat, width, height, 0, dataFormat, dataType, data);

	if (generatesMipMap)
	{
		// Automatically generate all the required mipmaps for the currently bound texture
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	// Unbind
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::BindTexture() const
{
	// Activate proper texture unit before binding
	glActiveTexture(GL_TEXTURE0 + mTextureUnit);

	// Bind the texture
	glBindTexture(GL_TEXTURE_2D, mTextureID);
}

void Texture::LoadTexture()
{
	// Load in texture file with stbi_load:
	unsigned char* data = stbi_load(mName.c_str(), &mWidth, &mHeight, &mNumChannels, 0);

	if (data)
	{
		std::cout << "Loading texture: " << mName << "\n";

		LOG_DEBUG("Loading texture file: " + mName);

		bool generatesMipMap = true;
		bool flipTexture = true;
		GLenum minFilter = GL_LINEAR_MIPMAP_LINEAR;
		if (mType == TextureType::Sprite || mType == TextureType::Font)
		{
			generatesMipMap = false;
			flipTexture = false;
			minFilter = GL_LINEAR;
		}

		GenerateTexture(GL_TEXTURE_2D, mWidth, mHeight, GL_UNSIGNED_BYTE, data, generatesMipMap, 
			flipTexture, mNumChannels, GL_REPEAT, GL_REPEAT, minFilter, GL_LINEAR);
	}
	else
	{
		LOG_WARNING("Failed to load texture: " + mName);
		std::cout << "Failed to load texture: " << mName << "\n";
	}

	// Free image data
	stbi_image_free(data);
}
