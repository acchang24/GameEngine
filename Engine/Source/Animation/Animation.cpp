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

	mData.reserve(static_cast<size_t>(animation->mNumChannels));
	mBones.reserve(static_cast<size_t>(animation->mNumChannels));

	ReadNodeHeirarchy(mRoot, rootNode, skeleton->GetBoneMap());
	ReadBones(animation, skeleton);
}

Animation::~Animation()
{
	std::cout << "Delete animation" << std::endl;

	for (auto& b : mBones)
	{
		delete b;
	}
}

void Animation::ReadNodeHeirarchy(AnimNode& dest, const aiNode* src, std::unordered_map<std::string, BoneData>& boneData)
{
	dest.name = src->mName.data;
	dest.transformation = AssimpGLMHelper::ConvertMatrixToGLMFormat(src->mTransformation);
	dest.numChildren = src->mNumChildren;
	dest.children.reserve(src->mNumChildren);

	if (boneData.find(dest.name) != boneData.end())
	{
		BoneData bd = boneData[dest.name];
		Bone* bone = new Bone(dest.name, bd.index, bd.offset);
		dest.bone = bone;
		mBones.emplace_back(bone);
		mData.emplace_back(dest);
	}
	else
	{
		mData.emplace_back(dest);
	}

	for (int i = 0; i < src->mNumChildren; ++i)
	{
		AnimNode newNode = {};
		ReadNodeHeirarchy(newNode, src->mChildren[i], boneData);
		dest.children.emplace_back(newNode);
	}
}

void Animation::ReadBones(const aiAnimation* anim, Skeleton* skeleton)
{
	int size = anim->mNumChannels;

	std::string boneName;

	for (int i = 0; i < size; ++i)
	{
		aiNodeAnim* channel = anim->mChannels[i];
		boneName = channel->mNodeName.data;

		if (boneName == mBones[i]->GetBoneName())
		{
			mBones[i]->ReadKeyFrames(channel);
		}
	}
}
