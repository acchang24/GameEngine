#include "Animation.h"
#include <iostream>
#include "../Util/AssimpGLMHelper.h"
#include "Skeleton.h"
#include "Bone.h"

Animation::Animation(const aiAnimation* animation, const aiNode* rootNode, Skeleton* skeleton) :
	mRoot({}),
	mName(animation->mName.C_Str()),
	mDuration(animation->mDuration),
	mTicksPerSecond(animation->mTicksPerSecond)
{
	skeleton->AddAnimation(this);

	aiMatrix4x4 globalTransformation = rootNode->mTransformation;
	globalTransformation = globalTransformation.Inverse();

	glm::mat4 transform = AssimpGLMHelper::ConvertMatrixToGLMFormat(globalTransformation);

	skeleton->SetGlobalInverseTransform(transform);

	std::vector<Bone*> bones;
	bones.reserve(static_cast<size_t>(animation->mNumChannels));
	mData.reserve(static_cast<size_t>(animation->mNumChannels));

	ReadNodeHeirarchy(mRoot, rootNode, skeleton->GetBoneMap(), bones);
	ReadBones(animation, bones);
}

Animation::~Animation()
{
	std::cout << "Delete animation" << std::endl;

	for (auto& d : mData)
	{
		delete d.bone;
	}
	mData.clear();
}

void Animation::ReadNodeHeirarchy(AnimNode& dest, const aiNode* src, std::unordered_map<std::string, BoneData>& boneData, std::vector<Bone*>& bones)
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
		bones.emplace_back(bone);
	}
	mData.emplace_back(dest);
	

	for (int i = 0; i < src->mNumChildren; ++i)
	{
		AnimNode newNode = {};
		ReadNodeHeirarchy(newNode, src->mChildren[i], boneData, bones);
		dest.children.emplace_back(newNode);
	}
}

void Animation::ReadBones(const aiAnimation* anim, std::vector<Bone*>& bones)
{
	int size = anim->mNumChannels;

	std::string boneName;

	for (int i = 0; i < size; ++i)
	{
		aiNodeAnim* channel = anim->mChannels[i];
		boneName = channel->mNodeName.data;

		if (boneName == bones[i]->GetBoneName())
		{
			bones[i]->ReadKeyFrames(channel);
		}
	}
}
