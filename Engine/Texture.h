#pragma once
#include <string>

// Enum class for the different types of textures
enum class TextureType
{
	Diffuse, // Normal texture
	Specular, // Specular maps
	Emission // Emission maps
};

// Enum class for texture units
enum class TextureUnit
{
	Default = 0, // Default texture unit, no texture should be applied to this unit
	FrameBuffer = 11, // Texture unit 11 is used to sample a texture created from a frame buffer
	CubeMap = 12, // Texture unit 12 is used to sample from a cube map

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