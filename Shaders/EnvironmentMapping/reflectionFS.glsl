// Specify OpenGL 4.2 with core functionality
#version 420 core

in vec3 fragPos;
in vec3 normal;
in vec3 viewPosition;

// Uniform for the cube map sampler
uniform samplerCube cubeMap;

out vec4 fragColor;

void main()
{
    // Calculate view direction for vector I
    vec3 I = normalize(fragPos - viewPosition);
    // Calculate reflection vector for vector R
    vec3 R = reflect(I, normalize(normal));
    // Use vector R as the direction vector to sample from the cubemap
    fragColor = vec4(texture(cubeMap, R).rgb, 1.0);
}
