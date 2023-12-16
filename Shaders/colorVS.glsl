// Specify OpenGL 4.2 with core functionality
#version 420 core

// position variable has attribute position 0
layout (location = 0) in vec3 position;
// color variable has attribute position 1
layout (location = 1) in vec4 color;

// Model matrix uniform
uniform mat4 model;
// ViewProjection matrix uniform
uniform mat4 viewProjection;

// Specify a vec4 color output to the fragment shader
out vec4 vertexColor;

void main()
{
	gl_Position = viewProjection * model * vec4(position, 1.0);
	
	// Set output variable color
	vertexColor = color;
}