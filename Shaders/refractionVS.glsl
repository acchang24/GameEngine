// Specify OpenGL 4.2 with core functionality
#version 420 core

// position variable has attribute position 0
layout (location = 0) in vec3 pos;

// normal variable has attribute position 1
layout (location = 1) in vec3 inNormal;


// Model matrix uniform
uniform mat4 model;
// ViewProjection matrix uniform
uniform mat4 viewProjection;

out vec3 position;
out vec3 normal;

void main()
{
    normal = mat3(transpose(inverse(model))) * inNormal;
    position = vec3(model * vec4(pos, 1.));
    gl_Position = viewProjection * vec4(position, 1.0);
}  
