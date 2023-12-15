// Specify OpenGL 4.2 with core functionality
#version 420 core

// Input variables from vertex shader call (same name and same type in vertex shader)
in vec4 vertexColor;
in vec2 textureCoord;

// Set a uniform for the 2D texture sampler
uniform sampler2D textureSampler;

// Final vector4 fragment color output
out vec4 fragColor;

void main() 
{
	// Sampler colors of a texture with texture function, passing in sampler and coordinates
	fragColor = texture(textureSampler, textureCoord) * vertexColor;
}