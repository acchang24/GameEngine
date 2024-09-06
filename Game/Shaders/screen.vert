// Specify OpenGL 4.2 with core functionality
#version 420 core

// position variable has attribute position 0
layout (location = 0) in vec2 position;

// texture variable has attribute position 1
layout (location = 1) in vec2 texCoord;

// Specify a vec2 texture output to the fragment shader
out vec2 textureCoord;

void main()
{
    // Multiply position by model and view/projection matrices
    gl_Position = vec4(position.x, position.y, 0.0, 1.0); 
    
    textureCoord = texCoord;
}