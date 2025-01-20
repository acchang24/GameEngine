// Specify OpenGL 4.5 with core functionality
#version 450 core

// Fragment shader input
in VS_OUT {
    vec3 fragPos;
    vec3 normal;
    vec3 viewPosition;
} fs_in;

// Uniform for the cube map sampler
uniform samplerCube cubeMap;

out vec4 fragColor;

void main()
{
    // Calculate view direction for vector I
    vec3 I = normalize(fs_in.fragPos - fs_in.viewPosition);
    // Calculate reflection vector for vector R
    vec3 R = reflect(I, normalize(fs_in.normal));
    // Use vector R as the direction vector to sample from the cubemap
    fragColor = vec4(texture(cubeMap, R).rgb, 1.0);
}
