#pragma once
#include "Light.h"

class Sphere;

// Struct for spotlight data that contains its position, direction,
// cutoff, outer cutoff, constant, linear, and quadratic terms.
struct SpotLightData
{
	glm::vec3 position;
	glm::vec3 direction;
	float cutoff;
	float outerCutoff;
	float constant;
	float linear;
	float quadratic;
};

// Spotlight is a type of light that shoots light rays
// in a specific direction. Objects within a certain radius
// will be lit.
class SpotLight : public Light
{
public:
	// SpotLight constructor
	// @param - const glm::vec4& for the light's color
	// @param - const glm::vec3& for the light's position
	// @param - const glm::vec3& for the light's direction
	// @param - float for the spotlight's inner circle cutoff
	// @param - float for the spotlight's outer circle cutoff
	// @param - float for attenuation constant term
	// @param - float for attenuation linear term
	// @param - float for attenuation quadratic term
	// @return - SpotLight* for a newly allocated SpotLight
	SpotLight(const glm::vec4& color, const glm::vec3& pos, const glm::vec3& dir, float cutoff, float outerCutoff, float constant, float linear, float quadratic);
	~SpotLight();

	// Override of SetLight, and sets spot light data in the shaders
	void SetLight() override;

	// Getter for the light sphere
	// @return - Sphere* for the spot light's sphere
	Sphere* GetLightSphere() { return mLightSphere; }

	// Spotlight data
	SpotLightData mSpotLightData;
private:
	// Sphere representing a spot light's source
	Sphere* mLightSphere;
};