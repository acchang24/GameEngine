#include "Animation.h"
#include "../Entity/Entity3D.h"
#include "../Util/AssimpGLMHelper.h"
#include "Skeleton.h"
#include <iostream>

Animation::Animation(const aiAnimation* animation, const aiNode* rootNode, Skeleton* skeleton) :
	mName(animation->mName.C_Str()),
	mDuration(animation->mDuration),
	mTicksPerSecond(animation->mTicksPerSecond)
{
	skeleton->AddAnimation(this);

	aiMatrix4x4 globalTransformation = rootNode->mTransformation;
	globalTransformation = globalTransformation.Inverse();

	glm::mat4 transform = AssimpGLMHelper::ConvertMatrixToGLMFormat(globalTransformation);

	skeleton->SetGlobalInverseTransform(transform);

	ReadNodeHeirarchy(mRoot, rootNode);
	ReadBones(animation, skeleton);
}

Animation::~Animation()
{
	std::cout << "Delete animation" << std::endl;
}

void Animation::ReadNodeHeirarchy(AssimpNode& dest, const aiNode* src)
{
	dest.name = src->mName.data;
	dest.transformation = AssimpGLMHelper::ConvertMatrixToGLMFormat(src->mTransformation);
	dest.numChildren = src->mNumChildren;
	dest.children.reserve(dest.numChildren);

	for (int i = 0; i < src->mNumChildren; ++i)
	{
		AssimpNode newNode = {};
		ReadNodeHeirarchy(newNode, src->mChildren[i]);
		dest.children.emplace_back(newNode);
	}
}

void Animation::ReadBones(const aiAnimation* anim, Skeleton* skeleton)
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
		mBones.emplace_back(Bone(boneName, boneInfoMap[boneName].index, boneInfoMap[boneName].offset, channel));
	}
	mBoneInfoMap = boneInfoMap;
}
