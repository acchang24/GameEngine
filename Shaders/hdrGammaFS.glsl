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

// Toggle hdr
uniform bool hdr;

// Toggle bloom
uniform bool bloom;

// HDR exposure
uniform float exposure;

void main()
{
    // Normal offscreen texture
    vec3 color = texture(screenTexture, textureCoord).rgb;

    if(bloom)
    {
        vec3 bloomColor = texture(blurTexture, textureCoord).rgb;
        color += bloomColor;
    }

    if(hdr)
    {
        vec3 hdrColor = color;

        // reinhard tone mapping
        // color = color / (color + vec3(1.0));

        // exposure tone mapping
        color = vec3(1.0) - exp(-hdrColor * exposure);
    }

    // Gamma correction
    color = pow(color, vec3(1.0 / 2.2));

    // final output
    fragColor = vec4(color, 1.0);
}