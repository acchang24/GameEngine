// Specify OpenGL 4.5 with core functionality
#version 450 core

// Fragment shader input
in VS_OUT {
	vec2 textureCoord;
} vs_in;

// Uniform sampler
uniform sampler2D sprite;

// Final fragment color
out vec4 fragColor;

void main() 
{
	fragColor = texture(sprite, vs_in.textureCoord);
}
