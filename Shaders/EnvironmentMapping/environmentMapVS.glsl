// Specify OpenGL 4.2 with core functionality
#version 420 core

// position variable has attribute position 0
layout (location = 0) in vec3 pos;
// normal variable has attribute position 1
layout (location = 1) in vec3 inNormal;

// Uniform buffer for camera's view * proj matrix and position
layout (std140, binding = 1) uniform CameraBuffer
{
    mat4 viewProjection;
    vec3 viewPos;
};

// Model matrix uniform
uniform mat4 model;

// Output a vec3 position to calculate view direction
out vec3 position;
// Output a vec3 normal
out vec3 normal;
// Pass the CameraBuffer's viewPos to fragment shader
out vec3 viewPosition;

void main()
{
    // Transform normal with inverse model matrix
    normal = mat3(transpose(inverse(model))) * inNormal;
    // Transform pos with model to get world-space position vector
    position = vec3(model * vec4(pos, 1.));
    // Transfrom by view * projection matrix
    gl_Position = viewProjection * vec4(position, 1.0);

    viewPosition = viewPos;
}  
