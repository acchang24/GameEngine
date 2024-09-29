// Specify OpenGL 4.2 with core functionality
#version 420 core

// Maximum number of bones a model can have
const int MAX_BONES = 100;
// Maximum number of bones that can affect a vertex
const int MAX_BONE_INFLUENCE = 4;

// position variable has attribute position 0
layout (location = 0) in vec3 position;
// boneIds variable has attribute position 5
layout (location = 5) in ivec4 boneIds;
// bone weights has attribute position 6
layout (location = 6) in vec4 weights;

// Uniform buffer for bone matrices
layout (std140, binding = 3) uniform SkeletonBuffer
{
	mat4 boneMatrices[MAX_BONES];
};

// Model matrix uniform
uniform mat4 model;

uniform bool isSkinned;

void main()
{
	vec4 pos;

	if(isSkinned)
	{
		vec4 totalPosition = vec4(0.0);
		for(int i = 0; i < MAX_BONE_INFLUENCE; ++i)
		{
			if(boneIds[i] == -1)
			{
				continue;
			}
			if(boneIds[i] >= MAX_BONES)
			{
				totalPosition = vec4(position, 1.0);
				break;
			}
			vec4 localPosition = boneMatrices[boneIds[i]] * vec4(position, 1.0);
			totalPosition += localPosition * weights[i];
		}
		pos = totalPosition;
	}
	else
	{
		pos = vec4(position, 1.0);
	}

	// Multiply position by model matrix
	gl_Position = model * pos;
}
