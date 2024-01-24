// Specify OpenGL 4.2 with core functionality
#version 420 core

// Maximum number of bones a model can have
const int MAX_BONES = 100;
// Maximum number of bones that can affect a vertex
const int MAX_BONE_INFLUENCE = 4;

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
// boneIds variable has attribute position 5
layout (location = 5) in ivec4 boneIds;
// bone weights has attribute position 6
layout (location = 6) in vec4 weights;

// Uniform buffer for camera's view * proj matrix and position
layout (std140, binding = 0) uniform CameraBuffer
{
    mat4 viewProjection;
    vec3 viewPos;
};

// Uniform buffer for bone matrices
layout (std140, binding = 3) uniform SkeletonBuffer
{
	mat4 boneMatrices[MAX_BONES];
};

// Model matrix uniform
uniform mat4 model;
// Matrix for light's perspective
uniform mat4 lightSpace;

// Specify a vec3 normal output to fragment shader
out vec3 normal;
// Specify a vec2 texture coordinates output to the fragment shader
out vec2 textureCoord;
// Specify vec3 for the fragment's position
out vec3 fragPos;
// Pass the CameraBuffer's viewPos to fragment shader
out vec3 viewPosition;
// fragment's position in light space
out vec4 fragPosLightSpace;
// Tangent, Bitangent, Normal matrix for normal mapping
out mat3 TBN;

void main()
{
	vec4 totalPosition = vec4(0.0);
	vec3 totalNormal = vec3(0.0);
	for(int i = 0; i < MAX_BONE_INFLUENCE; ++i)
	{
		if(boneIds[i] >= MAX_BONES)
		{
			totalPosition = vec4(position, 1.0);
			break;
		}
		vec4 localPosition = boneMatrices[boneIds[i]] * vec4(position, 1.0);
		totalPosition += localPosition * weights[i];
		vec3 localNormal = mat3(boneMatrices[boneIds[i]]) * inNormal;
		totalNormal += localNormal * weights[i];
	}

	// Multiply position by model and view/projection matrices
	gl_Position = viewProjection * model * totalPosition;

	// Multiply the vertex's normal attribute with the inverse model matrix
    // to transform to world space coordinates
    normal = mat3(transpose(inverse(model))) * totalNormal;

	// Set output variable color
	textureCoord = uv;

	// Multiply the vertex's position attribute with the model matrix 
    // to transform to world space coordinates and use it for the fragment's position
	fragPos = vec3(model * vec4(position, 1.0));

	viewPosition = viewPos;

	fragPosLightSpace = lightSpace * vec4(fragPos, 1.0);

	vec3 T = normalize(vec3(model * vec4(tangent,   0.0)));
	vec3 B = normalize(vec3(model * vec4(bitangent, 0.0)));
	vec3 N = normalize(vec3(model * vec4(inNormal,    0.0)));
	TBN = mat3(T, B, N);
}
