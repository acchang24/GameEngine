// Specify OpenGL 4.5 with core functionality
#version 450 core

// position variable has attribute position 0
layout (location = 0) in vec3 position;
// normal variable has attribute position 1
layout (location = 1) in vec3 inNormal;
// texture variable has attribute position 2
layout (location = 2) in vec2 uv;
// tangent variable has attribute position 3
layout (location = 3) in vec3 tangent;
// bitangent variable has attribute position 4
layout (location = 4) in vec3 bitangent;
// instance matrices
layout (location = 7) in mat4 instanceModelMatrix;

// Uniform buffer for camera's view * proj matrix and position
layout (std140, binding = 0) uniform CameraBuffer
{
    mat4 viewProjection;
    vec3 viewPos;
};

out VS_OUT {
	// Specify a vec3 normal output to fragment shader
	vec3 normal;
	// Specify a vec2 texture coordinates output to the fragment shader
	vec2 textureCoord;
	// Specify vec3 for the fragment's position
	vec3 fragPos;
	// Pass the CameraBuffer's viewPos to fragment shader
	vec3 viewPosition;
	// fragment's position in light space
	vec4 fragPosLightSpace;
	// Tangent, Bitangent, Normal matrix for normal mapping
	mat3 TBN;
} vs_out;

void main()
{
	// Multiply position by model and view/projection matrices
	gl_Position = viewProjection * instanceModelMatrix * vec4(position, 1.0);

	// Multiply the vertex's normal attribute with the inverse model matrix
    // to transform to world space coordinates
    vs_out.normal = mat3(transpose(inverse(instanceModelMatrix))) * inNormal;

	// Set output variable color
	vs_out.textureCoord = uv;

	// Multiply the vertex's position attribute with the model matrix 
    // to transform to world space coordinates and use it for the fragment's position
	vs_out.fragPos = vec3(instanceModelMatrix * vec4(position, 1.0));

	vs_out.viewPosition = viewPos;

	vec3 T = normalize(vec3(instanceModelMatrix * vec4(tangent,   0.0)));
	vec3 B = normalize(vec3(instanceModelMatrix * vec4(bitangent, 0.0)));
	vec3 N = normalize(vec3(instanceModelMatrix * vec4(inNormal,    0.0)));
	vs_out.TBN = mat3(T, B, N);
}
