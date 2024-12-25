// Specify OpenGL 4.5 with core functionality
#version 450

// Input color variable from vertex shader
in vec4 vertexColor;

// Specify a vec4 color output
out vec4 FragmentColor;

void main()
{
	// Set fragment color output to the vertex color
	FragmentColor = vertexColor;
}