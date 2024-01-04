// Specify OpenGL 4.2 with core functionality
#version 420 core

// Struct to define a different number of texture units
// Add more samplers to this struct when needed
struct TextureSamplers
{
    sampler2D diffuse1;
    sampler2D specular1;
	sampler2D emission1;
};

// Input variables from vertex shader call (same name and same type in vertex shader)
in vec2 textureCoord;

// Uniform buffer for materials
layout (std140, binding = 2) uniform MaterialBuffer
{
	vec4 diffuseColor;
	vec4 specularColor;
	float specularIntensity;
	float shininess;
	bool hasDiffuseTexture;
	bool hasSpecularTexture;
	bool hasEmissionTexture;
};

// Uniform for the 2D texture samplers
uniform TextureSamplers textureSamplers;

// Final vector4 fragment color output
out vec4 fragColor;

void main()
{
	fragColor = texture(textureSamplers.diffuse1, textureCoord);
}
