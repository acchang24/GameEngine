// Specify OpenGL 4.2 with core functionality
#version 420 core

in vec3 texCoord;

out vec4 fragColor;

uniform samplerCube skybox;

void main()
{
    fragColor = texture(skybox, texCoord);
}  
