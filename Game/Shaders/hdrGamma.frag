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
// Toggle hdr
uniform bool hdr;
// HDR exposure
uniform float exposure;

// Final fragment color
out vec4 fragColor;

void main()
{
    // Normal offscreen texture
    vec3 color = texture(screenTexture, fs_in.textureCoord).rgb;

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