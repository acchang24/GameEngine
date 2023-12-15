// Specify OpenGL 4.2 with core functionality
#version 420 core

// position variable has attribute position 0
layout (location = 0) in vec3 position;
// texture variable has attribute position 1
layout (location = 1) in vec2 uv;

// Specify a vec2 texture coordinates output to the fragment shader
out vec2 textureCoord;

void main()
{
	gl_Position = vec4(position, 1.0);
	// Set output variable color
	textureCoord = uv;
}