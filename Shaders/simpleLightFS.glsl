// Specify OpenGL 4.2 with core functionality
#version 420 core

// Input variables from vertex shader call (same name and same type in vertex shader)
in vec2 textureCoord;

// Set a uniform for the 2D texture sampler
uniform sampler2D textureSampler;

// Uniform for light color
uniform vec4 lightColor;

// Final vector4 fragment color output
out vec4 fragColor;

void main() 
{
	float ambientStrength = 0.1;
	vec4 ambientLight = ambientStrength * lightColor;

	// Sampler colors of a texture with texture function, passing in sampler and coordinates
	fragColor = ambientLight * texture(textureSampler, textureCoord);
}