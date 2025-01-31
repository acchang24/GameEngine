#pragma once
#include <string>

// Enum class for texture types and its corresponding texture unit
enum class TextureType
{
	None = 0, // Default to 0
	Diffuse = 1, // Texture unit 1 for diffuse color textures
	Specular = 2, // Texture unit 2 for specular map textures
	Emission = 3, // Texture unit 3 for emission map textures
	Normal = 4, // Texture unit 4 for normal map textures
	FrameBuffer = 11, // Texture unit 11 is used to sample a texture created from a frame buffer
	CubeMap = 12, // Texture unit 12 is used to sample from a cube map
	Shadow = 13, // Texture unit 13 is used to sample from a directional shadow/depth map
	PointShadow = 14 // Texture unit 14 is used to sample from a cube map for a point shadow/depth map
};

// The Texture class helps load image files with the
// stb_image loader and saves image details. All texture
// objects are referenced with an integer and provides
// functions to help set currently bound texture. These are textures 
// used for texture mapping in 3D. Do not use this for 2D sprites.
class Texture
{
public:
	// Texture constructor takes the name of a texture file
	// and creates an OpenGl texture object
	// @param - const std::string& for the texture file name
	// @param - TextureType for the type used for the texture
	Texture(const std::string& textureFile);
	~Texture();

	// Bind it to so any subsequent texture commands will use the currently bound texture
	// Binding after activating a texture unit will bind the texture to that unit
	// There is a minimum of 16 texture units to use (GL_TEXTURE0 to GL_TEXTURE15)
	void BindTexture() const;

	// Getter for the texture's ID
	// @return - unsinged int mTextureID;
	unsigned int GetID() const { return mTextureID; }

	// Getter for the texture type
	// @return - TextureType mType
	TextureType GetType() const { return mType; }

	// Setter for the texture type, then loads/generates the texture based on the type
	// @param - TextureType for the texture type
	void SetType(TextureType type);

private:
	// Binds a texture and uses stbi to load a texture based off the texture's file name.
	// Then the texture and mipmaps are generated based off of texture type
	void LoadTexture();

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
