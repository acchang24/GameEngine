// Specify OpenGL 4.2 with core functionality
#version 420 core

in vec3 texCoord;

out vec4 fragColor;

// Uniform for the cube map sampler
uniform samplerCube cubeMap;

void main()
{
    // Sample from cube map
    fragColor = texture(cubeMap, texCoord);
}  
