// Specify OpenGL 4.2 with core functionality
#version 420 core

in vec4 fragPos;

uniform vec3 lightPos;
uniform float farPlane;

void main()
{
    float lightDistance = length(fragPos.xyz - lightPos);

    // map to [0, 1] range by dividing by farPlane
    lightDistance = lightDistance / farPlane;

    // Write as modified depth
    gl_FragDepth = lightDistance;
}
