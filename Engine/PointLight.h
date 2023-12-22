#pragma once
#include "Light.h"

class Sphere;

// Struct for point light data that contains information
// about the light's position and attenuation terms
struct PointLightData
{
	glm::vec3 position;
	float constant;
	float linear;
	float quadratic;
};

class PointLight : public Light
{
public:
	// PointLight constructor takes in data for a point light, and creates a Sphere entity
	// to represent its light source.
	// @param - const glm::vec4& for the point light's color
	// @param - const glm::vec3& for the point light's position
	// @param - float for the light's constant term for attenuation
	// @param - float for the light's linear term for attenuation
	// @param - float for the light's quadratic term for attenuation
	PointLight(const glm::vec4& color, const glm::vec3& position, float constant, float linear, float quadratic);
	~PointLight();

	// Override of Light's SetLight function
	// Sends its light data as well as pointlight data
	// to the shader.
	void SetLight() override;

	// Getter for the light sphere
	// @return - Sphere* for the point light's sphere
	Sphere* GetLightSphere() { return mLightSphere; }

	// Point light data
	PointLightData mPointLightData;

private:
	// Sphere representing a point light's source
	Sphere* mLightSphere;
};
