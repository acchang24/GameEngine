#pragma once
#include <vector>
#include <string>

class Shader;

// CubeMap is a texture that containes 6 individual 2D textures
// that each form a side of a cube. This can be used to sampled
// using a directional vector to create things like skyboxes
// and environment mapping.
class CubeMap
{
public:
	// Cube map constructor
	// @param - const std::vector<std::string>& for the texture faces
	CubeMap(const std::vector<std::string>& textureFaces);
	~CubeMap();

	// Activates the texture unit used for cube maps, then locates
	// the appropriate sampler in the shader and binds the cube map's texture.
	void BindCubeMap() const;

	// Gets the texture id
	// @return - unsigned int for the texture's id
	unsigned int GetTextureID() const { return mTextureID; }

	// Gets the texture unit
	// @return - int for the texture unit
	int GetTextureUnit() const { return mTextureUnit; }

private:
	// Texture ID used for reference
	unsigned int mTextureID;

	// Int used for texture unit, this is initialized to TextureType::CubeMap
	int mTextureUnit;
};