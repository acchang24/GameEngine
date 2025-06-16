// Specify OpenGL 4.5 with core functionality
#version 450 core

// Uniform for color
uniform vec3 color;

// Final fragment color
out vec4 fragColor;

void main() 
{
	fragColor = vec4(color, 1.0);
}
