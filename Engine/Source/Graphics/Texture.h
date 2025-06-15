#pragma once
#include <string>
#include <glad/glad.h>

// Enum class for texture types and its corresponding texture unit
enum class TextureType
{
	None = 0,				// Default to 0
	Diffuse = 1,			// Texture unit 1 for diffuse color textures
	Specular = 2,			// Texture unit 2 for specular map textures
	Emission = 3,			// Texture unit 3 for emission map textures
	Normal = 4,				// Texture unit 4 for normal map textures
	FrameBuffer = 5,		// Texture unit 5 is used to sample a texture created from a frame buffer
	CubeMap = 6,			// Texture unit 6 is used to sample from a cube map
	Shadow = 7,				// Texture unit 7 is used to sample from a directional shadow/depth map
	PointShadow = 8,		// Texture unit 8 is used to sample from a cube map for a point shadow/depth map
	Sprite = 9,				// Texture unit 9 is used to sample from a texture that is used as a sprite for 2D rendering
	Font = 10,				// Texture unit 10 is used to sample from a texture that is used for fonts
};

// The Texture class helps load image files with the
// stb_image loader and saves image details. All texture
// objects are referenced with an integer and provides
// functions to help set currently bound texture. These are textures 
// used for texture mapping in 3D. 
class Texture
{
public:
	// Texture constructor: Generates texture and creates OpenGL texture object
	// @param - TextureType for the type used for the texture
	Texture(TextureType type);
	// Texture constructor: Generates texture with texture file
	// and creates an OpenGl texture object
	// @param - const std::string& for the texture file name
	// @param - TextureType for the type used for the texture
	Texture(const std::string& textureFile, TextureType type);
	~Texture();

	// Generates a texture to the currently bound texture
	// @param - GLenum for target texture. Typically use GL_TEXTURE_2D for textures, frame buffers and shadow maps (TextureTypes 1-5, 7, 9, 10)
	// and use GL_TEXTURE_CUBE_MAP_POSITIVE_X for cube maps, point shadow maps (TextureType 6, 8)
	// @param - int for texture width
	// @param - int for texture height
	// @param - GLenum for type. Specifies the data type of the pixel data. Use GL_UNSIGNED_BYTE for most applications. 
	// Use GL_FLOAT for shadow/point shadow maps
	// @param - const void* for the pointer to the image data
	// @param - bool for if this texture should automatically generate mip maps
	// @param - bool for flipping texture
	// @param - int for number of channels for this texture
	// @param - GLenum for the wrap s parameter
	// @param - GLenum for the wrap t parameter
	// @param - GLenum for min texture filtering
	// @param - GLenum for max texture filtering
	void GenerateTexture(GLenum target, int width, int height, GLenum dataType, const void* data,
		bool generatesMipMap, bool flipTexture, int numChannels, GLenum wrapS, GLenum wrapT, GLenum minFilter, GLenum maxFilter);

	// Bind it to so any subsequent texture commands will use the currently bound texture
	// Binding after activating a texture unit will bind the texture to that unit
	// There is a minimum of 16 texture units to use (GL_TEXTURE0 to GL_TEXTURE15)
	void BindTexture() const;

	// Gets the texture's file name
	// @return - const std::string& for the name of the file
	const std::string& GetName() const { return mName; }

	// Getter for the texture's ID
	// @return - unsigned int mTextureID;
	unsigned int GetID() const { return mTextureID; }

	// Gets the texture width
	// @return - float for width
	float GetWidth() const { return mWidth; }

	// Gets the texture height
	// @return - float for height
	float GetHeight() const { return mHeight; }

	// Gets the number of channels for this texture
	// @return - int for channel number
	int GetNumChannels() const { return mNumChannels; }

	// Getter for the texture unit
	// @return - int for the texture unit
	int GetTextureUnit() const { return mTextureUnit; }

	// Getter for the texture type
	// @return - TextureType mType
	TextureType GetType() const { return mType; }

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

	// Texture unit
	int mTextureUnit;

	// Type of the texture
	TextureType mType;
};
