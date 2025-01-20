// Specify OpenGL 4.5 with core functionality
#version 450 core

// position variable has attribute position 0
layout (location = 0) in vec3 position;

// Vertex shader output
out VS_OUT {
    vec3 texCoord;
} vs_out;

// uniform for view projection matrix
uniform mat4 viewProjection;

void main()
{
    // Use interpolated position vector as texture's direction vector to sample 
    vs_out.texCoord = position;
    // multilply position by view projection matrix
    vec4 pos = viewProjection * vec4(position, 1.0);
    // set z component of output position to w component so that z is always 1.0
    gl_Position = pos.xyww;
}