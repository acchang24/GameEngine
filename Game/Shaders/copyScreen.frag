// Specify OpenGL 4.5 with core functionality
#version 450 core

// Fragment shader input
in VS_OUT {
    vec2 textureCoord;
} fs_in;

// Uniform sampler
uniform sampler2D screenTexture;

// Final fragment color
out vec4 fragColor;

void main()
{
    // Normal offscreen texture
    vec3 color = texture(screenTexture, fs_in.textureCoord).rgb;

    fragColor = vec4(color, 1.0);
}