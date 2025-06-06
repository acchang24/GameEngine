#pragma once
#include "LightConstants.h"

class Renderer;
class UniformBuffer;

// Lights class manages all the lighting within a scene.
// Allocators for specific light casters can be used to
// add lights to a scene. This lighting data is stored
// and sent into a uniform buffer that the shaders can use.
class Lights
{
public:
	Lights();
	~Lights();

	// Creates a UniformBuffer for lights object to send lighting data to a buffer
	// @param - Renderer3D* to create/add a buffer to its map of UniformBuffers
	void CreateBuffer(Renderer* renderer);

	// Updates the lighting buffer with the current state of the light arrays
	void SetBuffer();

	// Gets the light arrays
	// @return - LightArrays& for the light arrays
	LightArrays& GetLights() { return mLightArrays; }

	// Gets the light buffer
	// @return - UniformBuffer* for the lighting buffer
	UniformBuffer* GetLightBuffer() { return mLightBuffer; }

	// Allocator for spot lights
	// @param - const glm::vec4& for the light's color
	// @param - const glm::vec3& for the light's position
	// @param - const glm::vec3& for the light's direction
	// @param - float for the spotlight's inner circle cutoff
	// @param - float for the spotlight's outer circle cutoff
	// @param - float for attenuation constant term
	// @param - float for attenuation linear term
	// @param - float for attenuation quadratic term
	// @return - SpotLight* for a newly allocated SpotLight
	SpotLight* AllocateSpotLight(const glm::vec4& color, const glm::vec3& pos, const glm::vec3& dir, float cutoff, float outerCutoff, float constant, float linear, float quadratic);

	// Allocator for point lights
	// @param - const glm::vec4& for the point light's color
	// @param - const glm::vec3& for the light's position
	// @param - float for attenuation constant term
	// @param - float for attenuation linear term
	// @param - float for attenuation quadratic term
	// @return - PointLight* for a newly allocated PointLight
	PointLight* AllocatePointLight(const glm::vec4& color, const glm::vec3& position, float constant, float linear, float quadratic);

	// Allocator for directional lights
	// @param - const glm::vec4& for the light's color
	// @param - const glm::vec3& for the light's direction
	// @return - DirectionalLight* a newly allocated DirectionalLight
	DirectionalLight* AllocateDirectionalLight(const glm::vec4& color, const glm::vec3& direction);

	// Loops through each light and sets them to disabled
	void DeAllocateLights();

private:
	// Array of different lights
	LightArrays mLightArrays;

	// Buffer to send lighting data to shaders
	UniformBuffer* mLightBuffer;
};