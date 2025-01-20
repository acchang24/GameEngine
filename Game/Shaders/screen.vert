// Specify OpenGL 4.5 with core functionality
#version 450 core

// position variable has attribute position 0
layout (location = 0) in vec2 position;
// texture variable has attribute position 1
layout (location = 1) in vec2 texCoord;

// Vertex shader output
out VS_OUT {
    vec2 textureCoord;
} vs_out;

void main()
{
    // Multiply position by model and view/projection matrices
    gl_Position = vec4(position.x, position.y, 0.0, 1.0); 
    
    vs_out.textureCoord = texCoord;
}