// Specify OpenGL 4.5 with core functionality
#version 450 core

// inPosition variable has attribute position 0
layout (location = 0) in vec3 inPosition;
// inColor variable has attribute position 1
layout (location = 1) in vec4 inColor;

// Specify a vec4 color output to the fragment shader
out vec4 vertexColor;

// Model matrix uniform
uniform mat4 model;

void main()
{
	gl_Position = model * vec4(inPosition, 1.0);

	// Set the output color for fragment shader
	vertexColor = inColor;
}
