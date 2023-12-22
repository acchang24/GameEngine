#pragma once
#include "Light.h"

// DirectionalLight is a light source that is modeled
// to have all its light come in a parallel direction.
class DirectionalLight : public Light
{
public:
	// Directional light constructor with direction
	// @param - const glm::vec3& for the light's direction
	DirectionalLight(const glm::vec3& dir);
	// Directional light constructor with color and direction
	// @param - const glm::vec4& for the light's color
	// @param - const glm::vec3& for the light's direction
	DirectionalLight(const glm::vec4& color, const glm::vec3& dir);
	~DirectionalLight();

	void SetLight() override;

	// The direction the light is facing
	glm::vec3 mDirection;
private:
};
