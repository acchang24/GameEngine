// Specify OpenGL 4.5 with core functionality
#version 450

// Input uv coordinate variable from vertex shader
in vec2 textureUV;

// Specify a vec4 color output
out vec4 FragmentColor;

uniform sampler2D diffuseSampler;

void main()
{
	vec3 color = texture(diffuseSampler, textureUV).rgb;

	// HDR ToneMapping
	color = vec3(1.0) - exp(-color * 1.0);
	
	// Gamma Correct
	color = pow(color, vec3(1.0 / 2.2));

	FragmentColor = vec4(color, 1.0);
}