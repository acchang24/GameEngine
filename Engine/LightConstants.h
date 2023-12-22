#pragma once
#include "glm/glm.hpp"
#include "DirectionalLight.h"
#include "PointLight.h"

const int MAX_LIGHTS = 10;

// Struct that contains different arrays for 
// directional lights, point lights, and spotlights.
// Each array is capped to a certain number that is allowed
// to be rendered into the scene.
struct LightArrays
{
    // Array of directional lights
    DirectionalLight* mDirectionalLights[MAX_LIGHTS];
    // Array of point lights
    PointLight* mPointLights[MAX_LIGHTS];
};
