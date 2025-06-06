// Specify OpenGL 4.5 with core functionality
#version 450 core

// position variable has attribute position 0
layout (location = 0) in vec2 position;
// texture variable has attribute position 1
layout (location = 1) in vec2 uv;

// Model matrix uniform
uniform mat4 model;
// Projection matrix uniform
uniform mat4 projection;

// Vertex shader output
out VS_OUT {
	// Specify a vec2 texture output
	vec2 textureCoord;
} vs_out;

void main()
{
	gl_Position = projection * model * vec4(position, 0.0, 1.0);
	
	vs_out.textureCoord = uv;
}
