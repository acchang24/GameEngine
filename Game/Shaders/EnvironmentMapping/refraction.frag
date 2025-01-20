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
    // Calculate ratio with refractive indices
    float ratio = 1.0 / 1.52;
    // Calculate view direction for vector I
    vec3 I = normalize(fs_in.fragPos - fs_in.viewPosition);
    // Calculate refraction vector for vector R
    vec3 R = refract(I, normalize(fs_in.normal), ratio);
    // Use vector R as the direction vector to sample from the cubemap
    fragColor = vec4(texture(cubeMap, R).rgb, 1.0);
}
