// Specify OpenGL 4.2 with core functionality
#version 420 core

// position variable has attribute position 0
layout (location = 0) in vec3 position;

// Specify a vec4 color output to the fragment shader
out vec4 vertexColor;

void main()
{
	gl_Position = vec4(position, 1.0);
	// Set output variable color
	vertexColor = vec4(0.1, 0.6, 0.9, 1.0);
}