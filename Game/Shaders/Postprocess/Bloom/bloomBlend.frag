// Specify OpenGL 4.2 with core functionality
#version 420 core

// Vec2 input from vertex shader
in vec2 textureCoord;

// Uniform sampler for the regular framebuffer image
uniform sampler2D screenTexture;
// Uniform sampler for the blur framebuffer image
uniform sampler2D blurTexture;

// Specify a vec4 output
out vec4 fragColor;

// Toggle bloom
uniform bool bloom;

void main()
{
    // Normal offscreen texture
    vec3 color = texture(screenTexture, textureCoord).rgb;

    if(bloom)
    {
        vec3 bloomColor = texture(blurTexture, textureCoord).rgb;
        color += bloomColor;
    }

    // final output
    fragColor = vec4(color, 1.0);
}