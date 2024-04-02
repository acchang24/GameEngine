#include "Animation.h"
#include <iostream>
#include "../Util/AssimpGLMHelper.h"
#include "Bone.h"
#include "Skeleton.h"

Animation::Animation(const aiAnimation* animation, const aiNode* rootNode, Skeleton* skeleton) :
	mRoot({}),
	mGlobalInverseTransform(1.0f),
	mName(animation->mName.C_Str()),
	mDuration(animation->mDuration),
	mTicksPerSecond(animation->mTicksPerSecond)
{
	skeleton->AddAnimation(this);

	aiMatrix4x4 globalTransformation = rootNode->mTransformation;
	globalTransformation = globalTransformation.Inverse();

	mGlobalInverseTransform = AssimpGLMHelper::ConvertMatrixToGLMFormat(globalTransformation);

	mBones.reserve(static_cast<size_t>(animation->mNumChannels));

	ReadNodeHeirarchy(mRoot, rootNode, skeleton->mBoneDataMap);
	ReadBones(animation);
}

Animation::~Animation()
{
	for (auto& b : mBones)
	{
		delete b;
	}
	mBones.clear();

	std::cout << "Deleted animation at " << this << "\n";
}

void Animation::ReadNodeHeirarchy(AnimNode& curr, const aiNode* src, std::unordered_map<std::string, BoneData>& boneData)
{
	curr.name = src->mName.data;
	curr.transformation = AssimpGLMHelper::ConvertMatrixToGLMFormat(src->mTransformation);
	curr.numChildren = src->mNumChildren;
	curr.children.reserve(src->mNumChildren);

	if (boneData.find(curr.name) != boneData.end())
	{
		BoneData bd = boneData[curr.name];
		Bone* bone = new Bone(curr.name, bd.index, bd.offset);
		curr.bone = bone;
		mBones.emplace_back(bone);
	}
	
	for (int i = 0; i < src->mNumChildren; ++i)
	{
		AnimNode newNode = {};
		ReadNodeHeirarchy(newNode, src->mChildren[i], boneData);
		curr.children.emplace_back(newNode);
	}
}

void Animation::ReadBones(const aiAnimation* anim)
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
