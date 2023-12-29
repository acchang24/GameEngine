// Specify OpenGL 4.2 with core functionality
#version 420 core

in vec3 position;
in vec3 normal;

// Uniform for the view position (camera position)
uniform vec3 viewPos;
// Uniform for the cube map sampler
uniform samplerCube cubeMap;

out vec4 fragColor;

void main()
{
    // Calculate view direction for vector I
    vec3 I = normalize(position - viewPos);
    // Calculate reflection vector for vector R
    vec3 R = reflect(I, normalize(normal));
    // Use vector R as the direction vector to sample from the cubemap
    fragColor = vec4(texture(cubeMap, R).rgb, 1.0);
}
