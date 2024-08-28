// Specify OpenGL 4.2 with core functionality
#version 420 core

// Vec2 input from vertex shader
in vec2 textureCoord;

// Uniform sampler
uniform sampler2D screenTexture;

out vec4 fragColor;

void main()
{
    // Normal offscreen texture
    vec3 color = texture(screenTexture, textureCoord).rgb;

    // final output
    fragColor = vec4(color, 1.0);
}