// Specify OpenGL 4.2 with core functionality
#version 420 core

// position variable has attribute position 0
layout (location = 0) in vec3 position;
// color variable has attribute position 1
layout (location = 1) in vec4 color;
// texture variable has attribute position 2
layout (location = 2) in vec2 uv;

// Vec4 output for color
out vec4 vertexColor;
// Specify a vec2 texture coordinates output to the fragment shader
out vec2 textureCoord;

void main()
{
	gl_Position = vec4(position, 1.0);

	// Set output color
	vertexColor = color;
	// Set output texture coordinates
	textureCoord = uv;
}