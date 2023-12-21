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

class Light
{
public:
	Light();
	virtual ~Light();

protected:

};
