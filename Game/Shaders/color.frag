// Specify OpenGL 4.5 with core functionality
#version 450 core

// Fragment shader input
in VS_OUT {
	vec4 vertexColor;
} fs_in;

// Final fragment color
out vec4 fragColor;

void main() 
{
	fragColor = fs_in.vertexColor;
}