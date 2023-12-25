#pragma once
#include <vector>
#include <string>

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

	// Gets the texture id
	// @return - unsigned int for the texture's id
	unsigned int GetTextureID() const { return mTextureID; }

private:
	// Texture ID used for reference
	unsigned int mTextureID;
};