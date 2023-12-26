// Specify OpenGL 4.2 with core functionality
#version 420 core

in vec3 position;
in vec3 normal;

uniform vec3 viewPos;
uniform samplerCube skybox;

out vec4 fragColor;

void main()
{
    float ratio = 1.0 / 1.52;
    vec3 I = normalize(position - viewPos);
    vec3 R = refract(I, normalize(normal), ratio);
    fragColor = vec4(texture(skybox, R).rgb, 1.0);
}
