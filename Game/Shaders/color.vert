// Specify OpenGL 4.5 with core functionality
#version 450 core

// position variable has attribute position 0
layout (location = 0) in vec3 position;
// color variable has attribute position 1
layout (location = 1) in vec4 color;

// Uniform buffer for camera's view * proj matrix and position
layout (std140, binding = 0) uniform CameraBuffer
{
    mat4 viewProjection;
    vec3 viewPos;
};

// Model matrix uniform
uniform mat4 model;

// Vertex shader output
out VS_OUT {
	// Specify a vec4 color output
	vec4 vertexColor;
} vs_out;

void main()
{
	gl_Position = viewProjection * model * vec4(position, 1.0);
	
	vs_out.vertexColor = color;
}