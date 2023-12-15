// Specify OpenGL 4.2 with core functionality
#version 420 core

// Input variables from vertex shader call (same name and same type in vertex shader)
in vec4 vertexColor;

uniform vec4 newColor;

out vec4 fragColor;

void main() 
{
	fragColor = vertexColor;
	//fragColor = newColor;
}