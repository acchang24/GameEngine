// Specify OpenGL 4.5 with core functionality
#version 450 core

// Fragment shader input
in VS_OUT {
    vec2 textureCoord;
} fs_in;

// Uniform sampler
uniform sampler2D screenTexture;

// Pixels that are bright
out vec4 brightColor;

void main()
{
    vec3 color = texture(screenTexture, fs_in.textureCoord).rgb;

    // Texture for bright spots
    float brightness = dot(color, vec3(0.2126, 0.7152, 0.0722));

    if(brightness >= 1.0)
    {
        brightColor = vec4(color, 1.0);
    }
    else
    {
        brightColor = vec4(0.0, 0.0, 0.0, 1.0);
    }
}