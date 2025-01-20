// Specify OpenGL 4.5 with core functionality
#version 450 core

// Fragment shader input
in VS_OUT {
    vec2 textureCoord;
} fs_in;

// Uniform sampler
uniform sampler2D screenTexture;
// Uniform for the window's height
uniform float height;

// Blur pixels vertically
out vec4 fragColor;

void main()
{
    const float offset[5] = float[]( 0.0, 1.0, 2.0, 3.0, 4.0 );
    const float weight[5] = float[]( 0.2270270270, 0.1945945946, 0.1216216216, 0.0540540541, 0.0162162162 );

    vec3 color = texture(screenTexture, fs_in.textureCoord).rgb * weight[0];

    // Vertical blur
    for(int i = 1; i < 5; ++i)
    {
        color += texture(screenTexture, fs_in.textureCoord + vec2(0.0, offset[i]) / height).rgb * weight[i];
        color += texture(screenTexture, fs_in.textureCoord - vec2(0.0, offset[i]) / height).rgb * weight[i];
    }

    fragColor = vec4(color, 1.0);
}