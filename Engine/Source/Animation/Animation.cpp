#include "Animation.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include "../Entity/Entity3D.h"
#include "../Util/AssimpGLMHelper.h"
#include "Skeleton.h"

Animation::Animation(const std::string& animPath, Skeleton* skeleton)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(animPath, aiProcess_Triangulate);

	aiAnimation* animation = scene->mAnimations[0];
	mDuration = animation->mDuration;
	mTicksPerSecond = animation->mTicksPerSecond;

	aiMatrix4x4 globalTransformation = scene->mRootNode->mTransformation;
	globalTransformation = globalTransformation.Inverse();

	ReadNodeHeirarchy(mRoot, scene->mRootNode);
	ReadMissingBones(animation, skeleton);
}

Animation::~Animation()
{
}

void Animation::ReadNodeHeirarchy(AssimpNode& dest, const aiNode* src)
{
	dest.name = src->mName.data;
	dest.transformation = AssimpGLMHelper::ConvertMatrixToGLMFormat(src->mTransformation);
	dest.numChildren = src->mNumChildren;

	for (int i = 0; i < src->mNumChildren; ++i)
	{
		AssimpNode newNode = {};
		ReadNodeHeirarchy(newNode, src->mChildren[i]);
		dest.children.emplace_back(newNode);
	}
}

void Animation::ReadMissingBones(const aiAnimation* anim, Skeleton* skeleton)
{
	int size = anim->mNumChannels;

	auto& boneInfoMap = skeleton->GetBoneMap();
	int& boneCount = skeleton->GetNumBones();

	std::string boneName;

	for (int i = 0; i < size; ++i)
	{
		aiNodeAnim* channel = anim->mChannels[i];
		boneName = channel->mNodeName.data;

		if (boneInfoMap.find(boneName) == boneInfoMap.end())
		{
			boneInfoMap[boneName].index = boneCount;
			++boneCount;
		}
		mBones.emplace_back(Bone(channel->mNodeName.data, boneInfoMap[channel->mNodeName.data].index, channel));

	}
	mBoneInfoMap = boneInfoMap;
}
