#version 450 core

out vec4 fragColor;

// Fragment shader input
in VS_OUT {
    vec2 textureCoord;
} fs_in;

uniform sampler2D depthMap;
uniform float nearPlane;
uniform float farPlane;

// required when using a perspective projection matrix
float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // Back to NDC 
    return (2.0 * nearPlane * farPlane) / (farPlane + nearPlane - z * (farPlane - nearPlane));	
}

void main()
{             
    float depthValue = texture(depthMap, fs_in.textureCoord).r;
    //fragColor = vec4(vec3(LinearizeDepth(depthValue) / farPlane), 1.0); // perspective
    fragColor = vec4(vec3(depthValue), 1.0); // orthographic
}