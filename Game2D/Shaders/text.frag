// Specify OpenGL 4.5 with core functionality
#version 450 core

// Fragment shader input
in VS_OUT {
	vec2 textureCoord;
} fs_in;

// Uniform sampler
uniform sampler2D text;

// Uniform for color
uniform vec3 textColor;

// Final fragment color
out vec4 fragColor;

void main() 
{
	fragColor = vec4(textColor, 1.0) * vec4(1.0, 1.0, 1.0, texture(text, fs_in.textureCoord).r);
}
