// Specify OpenGL 4.5 with core functionality
#version 450

// inPosition variable has attribute position 0
layout (location = 0) in vec3 inPosition;
// inUV variable has attribute position 1
layout (location = 1) in vec2 inUV;

// Specify vec2 uv coordinate output for fragment shader
out vec2 textureUV;

// Uniform buffer for camera's view * proj matrix and position
layout (std140, binding = 0) uniform CameraBuffer
{
    mat4 viewProjection;
    vec3 viewPos;
};

// Model matrix uniform
uniform mat4 model;

void main()
{
	gl_Position = viewProjection * model * vec4(inPosition, 1.0);

	textureUV = inUV;
}