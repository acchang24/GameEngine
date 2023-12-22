#pragma once
#include "glm/glm.hpp"

// Struct that defines lighting data
// such as color, ambient, diffuse, and
// specular intensities
struct LightData
{
	glm::vec4 color;
	float ambientIntensity;
	float diffuseIntensity;
	float specularIntensity;
	bool isEnabled;
};

// Light class represents anything that can light a scene
// or objects. Any lighting data is held in the LightData
// struct, which gets sent to the shaders
class Light
{
public:
	Light();
	virtual ~Light();

	// Sends light data to the shader
	virtual void SetLight() const;

	// This light's data
	LightData mData;

protected:

};
