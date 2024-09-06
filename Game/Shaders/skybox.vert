// Specify OpenGL 4.2 with core functionality
#version 420 core

// position variable has attribute position 0
layout (location = 0) in vec3 position;

// Uniform buffer for camera's view * proj matrix and position
//layout (std140, binding = 1) uniform CameraBuffer
//{
//    mat4 viewProjection;
//    vec3 viewPos;
//};

// ouput a vec3 texture coordinate
out vec3 texCoord;

// uniform for view projection matrix
uniform mat4 viewProjection;

void main()
{
    // Use interpolated position vector as texture's direction vector to sample 
    texCoord = position;
    // multilply position by view projection matrix
    vec4 pos = viewProjection * vec4(position, 1.0);
    // set z component of output position to w component so that z is always 1.0
    gl_Position = pos.xyww;
}  
