// Specify OpenGL 4.2 with core functionality
#version 420 core

// Vec2 input from vertex shader
in vec2 textureCoord;

// Uniform sampler
uniform sampler2D screenTexture;

// Specify a vec4 output
out vec4 fragColor;

// Toggle hdr
uniform bool hdr;

void main()
{
    // Normal offscreen texture
    vec3 color = texture(screenTexture, textureCoord).rgb;

    if(hdr)
    {
        // reinhard tone mapping
        // color = color / (color + vec3(1.0));

        // exposure tone mapping
        color = vec3(1.0) - exp(-color * 1.0);
    }

    // Gamma correction
    color = pow(color, vec3(1.0 / 2.2));

    // final output
    fragColor = vec4(color, 1.0);
}