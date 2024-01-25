#pragma once
#include <unordered_map>
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <assimp/scene.h>
#include "../Graphics/VertexLayouts.h"

struct BoneData
{
	// bone's index for the bone matrices
	int index;
	// offset matrix transforms a vertex from model space to bone space (sets model's pose)
	glm::mat4 offset;
};

class Skeleton
{
public:
	Skeleton();
	~Skeleton();

	void ExtractVertexBoneWeights(std::vector<Vertex>& vertices, aiMesh* mesh, const aiScene* scene);

	void SetVertexBoneData(Vertex& vertex, int boneID, float weight);

	std::unordered_map<std::string, BoneData>& GetBoneMap() { return mBoneMap; }


private:
	std::unordered_map<std::string, BoneData> mBoneMap;

	int mNumBones;
};