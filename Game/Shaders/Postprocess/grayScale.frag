// Specify OpenGL 4.3 with core functionality
#version 430 core

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
    // Add gray scale to screen
    fragColor = texture(screenTexture, fs_in.textureCoord);
    float average = 0.2126 * fragColor.r + 0.7152 * fragColor.g + 0.0722 * fragColor.b;
    fragColor = vec4(average, average, average, 1.0);
}