#pragma once
#include <string>

// Enum class for the different types of textures
enum class TextureType
{
	Diffuse, // Normal texture
	Specular // Specular maps
};

// The Texture class helps load image files with the
// stb_image loader and saves image details. All texture
// objects are referenced with an integer and provides
// functions to help set currently bound texture.
class Texture
{
public:
	// Texture constructor takes the name of a texture file
	// and creates an OpenGl texture object
	// @param - const std::string& for the texture file name
	Texture(const std::string& textureFile);
	~Texture();

	// Binds the texture as the current one
	void SetActive();

	// Getter for the texture's ID
	// @return - unsinged int mTextureID;
	unsigned int GetID() const { return mTextureID; }

	// Getter for the texture type
	// @return - TextureType mType
	TextureType GetType() const { return mType; }

	// Setter for the texture type
	// @param - TextureType for the texture type
	void SetType(TextureType type) { mType = type; }

private:
	// Texture name (file path to the texture)
	std::string mName;

	// Texture id used for reference
	unsigned int mTextureID;

	// Width of the texture
	int mWidth;

	// Height of the texture
	int mHeight;

	// Number of color channels
	int mNumChannels;

	// Type of the texture
	TextureType mType;
};