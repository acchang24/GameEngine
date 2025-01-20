// Specify OpenGL 4.5 with core functionality
#version 450 core

// Fragment shader input
in VS_OUT {
    vec3 texCoord;
} vs_in;

// Uniform for the cube map sampler
uniform samplerCube cubeMap;

// Final fragment color
out vec4 fragColor;

void main()
{
    // Sample from cube map
    fragColor = texture(cubeMap, vs_in.texCoord);
}  
