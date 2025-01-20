// Specify OpenGL 4.5 with core functionality
#version 450 core

// Fragment shader input
in VS_OUT {
    vec2 textureCoord;
} fs_in;

// Uniform sampler for the regular framebuffer image
uniform sampler2D screenTexture;
// Uniform sampler for the blur framebuffer image
uniform sampler2D blurTexture;
// Toggle bloom
uniform bool bloom;

// Specify a vec4 output
out vec4 fragColor;

void main()
{
    // Normal offscreen texture
    vec3 color = texture(screenTexture, fs_in.textureCoord).rgb;

    if(bloom)
    {
        vec3 bloomColor = texture(blurTexture, fs_in.textureCoord).rgb;
        color += bloomColor;
    }

    // final output
    fragColor = vec4(color, 1.0);
}