// Specify OpenGL 4.5 with core functionality
#version 450 core

// position variable has attribute position 0
layout (location = 0) in vec3 position;
// normal variable has attribute position 1
layout (location = 1) in vec3 inNormal;
// texture variable has attribute position 2
layout (location = 2) in vec2 uv;

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
	// Specify a vec2 texture coordinates output to the fragment shader
	vec2 textureCoord;
} vs_out;

void main()
{
	// Multiply position by model and view/projection matrices
	gl_Position = viewProjection * model * vec4(position, 1.0);

	// Set output variable color
	vs_out.textureCoord = uv;
}
