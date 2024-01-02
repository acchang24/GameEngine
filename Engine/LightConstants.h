#pragma once
#include <glm/glm.hpp>

const int MAX_LIGHTS = 10;
const int MAX_DIR_LIGHT = 1;

// Struct that defines lighting data
// such as color, ambient, diffuse, and
// specular intensities
struct LightData
{
    glm::vec4 color;
    float ambientIntensity = 0.1f;
    float diffuseIntensity = 1.0f;
    float specularIntensity = 0.5f;
    bool isEnabled = false;
};

// Struct for point light data that contains information
// about the light's position and attenuation terms
struct PointLight
{
    LightData data;
    glm::vec3 position;
    float constant;
    float linear;
    float quadratic;
    float padding;
    float padding2;
};

// Struct for directional light contains info about a light's direction
struct DirectionalLight
{
    LightData data;
    glm::vec3 direction;
    float padding;
};

// Struct for spotlight data that contains its position, direction,
// cutoff, outer cutoff, constant, linear, and quadratic terms.
struct SpotLight
{
    LightData data;
    glm::vec3 position;
    float cutoff;
    glm::vec3 direction;
    float outerCutoff;
    float constant;
    float linear;
    float quadratic;
    float padding;
};

// Struct that contains different arrays for 
// directional lights, point lights, and spotlights.
// Each array is capped to a certain number that is allowed
// to be rendered into the scene.
struct LightArrays
{
    SpotLight spotLights[MAX_LIGHTS];
    PointLight pointLights[MAX_LIGHTS];
    DirectionalLight directionalLight[MAX_DIR_LIGHT];
};
