// Specify OpenGL 4.2 with core functionality
#version 420 core

// Vec2 input from vertex shader
in vec2 textureCoord;

// Uniform sampler
uniform sampler2D screenTexture;

// Specify a vec4 output
out vec4 fragColor;

void main()
{
    // Normal offscreen texture
    fragColor = texture(screenTexture, textureCoord);
}