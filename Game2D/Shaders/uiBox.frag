// Specify OpenGL 4.5 with core functionality
#version 450 core

// Uniform for color
uniform vec4 color;

// Final fragment color
out vec4 fragColor;

void main() 
{
	fragColor = color;
}
