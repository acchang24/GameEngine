// Specify OpenGL 4.5 with core functionality
#version 450 core

// position variable has attribute position 0
layout (location = 0) in vec2 position;

// Model matrix uniform
uniform mat4 model;
// Projection matrix uniform
uniform mat4 projection;

void main()
{
	gl_Position = projection * model * vec4(position, 0.0, 1.0);
}
