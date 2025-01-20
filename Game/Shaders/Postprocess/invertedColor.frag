// Specify OpenGL 4.5 with core functionality
#version 450 core

// Fragment shader input
in VS_OUT {
    vec2 textureCoord;
} fs_in;

// Uniform sampler
uniform sampler2D screenTexture;

// Specify a vec4 output
out vec4 fragColor;

void main()
{
    // invert the colors
    fragColor = vec4(vec3(1.0 - texture(screenTexture, fs_in.textureCoord)), 1.0);
}