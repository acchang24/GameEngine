#pragma once
#include <glm/glm.hpp>

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
	// Light constructor
	Light();
	// Light constructor with color
	// @param - const glm::vec4 for the light's color
	Light(const glm::vec4& color);
	// Light constructor with all light attributes
	// @param - const glm::vec4 for the light's color
	// @param - float for ambient intensity
	// @param - float for diffuse intensity
	// @param - float for specular intensity
	Light(const glm::vec4& color, float ambient, float diffuse, float specular);
	virtual ~Light();

	// Sends light data to the shader (overridable)
	virtual void SetLight();

	void SetIsEnabled(bool enabled) { mData.isEnabled = enabled; }

	bool IsEnabled() const { return mData.isEnabled; }

	// This light's data
	LightData mData;

	// The light's index in the array of lights
	unsigned int mIndex;

protected:
};
