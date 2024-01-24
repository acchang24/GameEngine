// Specify OpenGL 4.2 with core functionality
#version 420 core

// position variable has attribute position 0
layout (location = 0) in vec3 position;

// Model matrix uniform
uniform mat4 model;

uniform mat4 lightSpace;

void main()
{
	// Multiply position by model and view/projection matrices
	// Then transform to light space
	gl_Position = lightSpace * model * vec4(position, 1.0);
}
