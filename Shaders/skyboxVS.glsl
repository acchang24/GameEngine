// Specify OpenGL 4.2 with core functionality
#version 420 core

// position variable has attribute position 0
layout (location = 0) in vec3 position;

out vec3 texCoord;

uniform mat4 viewProjection;

void main()
{
    texCoord = position;
    vec4 pos = viewProjection * vec4(position, 1.0);
    gl_Position = pos.xyww;
}  
