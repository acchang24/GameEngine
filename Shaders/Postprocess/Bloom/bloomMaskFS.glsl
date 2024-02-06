// Specify OpenGL 4.2 with core functionality
#version 420 core

// Vec2 input from vertex shader
in vec2 textureCoord;

// Uniform sampler
uniform sampler2D screenTexture;

layout (location = 0) out vec4 fragColor;
layout (location = 1) out vec4 brightColor;

void main()
{
    vec3 color = texture(screenTexture, textureCoord).rgb;

     // Normal full screen pass
    fragColor = vec4(color, 1.0);


    // Texture for bright spots
    float brightness = dot(fragColor.rgb, vec3(0.2126, 0.7152, 0.0722));

    if(brightness > 1.0)
    {
        brightColor = vec4(fragColor.rgb, 1.0);
    }
    else
    {
        brightColor = vec4(0.0, 0.0, 0.0, 1.0);
    }
}