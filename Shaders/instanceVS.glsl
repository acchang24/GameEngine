// Specify OpenGL 4.2 with core functionality
#version 420 core

// position variable has attribute position 0
layout (location = 0) in vec3 position;
// normal variable has attribute position 1
layout (location = 1) in vec3 inNormal;
// texture variable has attribute position 2
layout (location = 2) in vec2 uv;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec4 bitangent;
// instance matrices
layout (location = 5) in mat4 instanceModelMatrix;

// Uniform buffer for camera's view * proj matrix and position
layout (std140, binding = 0) uniform CameraBuffer
{
    mat4 viewProjection;
    vec3 viewPos;
};

// Specify a vec3 normal output to fragment shader
out vec3 normal;
// Specify a vec2 texture coordinates output to the fragment shader
out vec2 textureCoord;
// Specify vec3 for the fragment's position
out vec3 fragPos;
// Pass the CameraBuffer's viewPos to fragment shader
out vec3 viewPosition;

void main()
{
	// Multiply position by model and view/projection matrices
	gl_Position = viewProjection * instanceModelMatrix * vec4(position, 1.0);

	// Multiply the vertex's normal attribute with the inverse model matrix
    // to transform to world space coordinates
    normal = mat3(transpose(inverse(instanceModelMatrix))) * inNormal;

	// Set output variable color
	textureCoord = uv;

	// Multiply the vertex's position attribute with the model matrix 
    // to transform to world space coordinates and use it for the fragment's position
	fragPos = vec3(instanceModelMatrix * vec4(position, 1.0));

	viewPosition = viewPos;
}
