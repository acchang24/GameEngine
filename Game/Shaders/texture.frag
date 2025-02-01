// Specify OpenGL 4.5 with core functionality
#version 450 core

// Struct to define a different number of texture units
// Add more samplers to this struct when needed
struct TextureSamplers
{
    sampler2D diffuse;
    sampler2D specular;
	sampler2D emission;
	sampler2D normal;
};

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
	bool hasNormalTexture;
};

// Fragmemt shader input
in VS_OUT {
	vec2 textureCoord;
} vs_in;

// Uniform for the 2D texture samplers
uniform TextureSamplers textureSamplers;

// Final vector4 fragment color output
out vec4 fragColor;

void main()
{
	fragColor = texture(textureSamplers.diffuse, vs_in.textureCoord);
}
