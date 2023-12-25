// Specify OpenGL 4.2 with core functionality
#version 420 core

in vec3 position;
in vec3 normal;

uniform vec3 viewPos;
uniform samplerCube skybox;

out vec4 fragColor;

void main()
{
    vec3 I = normalize(position - viewPos);
    vec3 R = reflect(I, normalize(normal));
    fragColor = vec4(texture(skybox, R).rgb, 1.0);
}
