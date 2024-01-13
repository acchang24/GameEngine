#pragma once
#include "Material.h"

class CubeMap;

// MaterialCubeMap is like a material class but instead
// uses a cube map as its texture
class MaterialCubeMap : public Material
{
public:
	MaterialCubeMap();
	MaterialCubeMap(const MaterialColors& mats);
	~MaterialCubeMap();

	// Sets the material's shader as active. It also binds 
	// the cube map onto its texture units and sends the MaterialColors
	// struct to the shader.
	void SetActive() override;

	// Gets the material's cube map
	// @return - CubeMap* for the material's cube map
	CubeMap* GetCubeMap() { return mCubeMap; }

	// Sets the material's cube map
	// @param - CubeMap* for the new cube map
	void SetCubeMap(CubeMap* cm) { mCubeMap = cm; }

private:
	// Cube map used for this material
	CubeMap* mCubeMap;
};
