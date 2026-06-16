#include "Model.h"
#include <iostream>
#include "../Graphics/Mesh.h"
#include "../Graphics/VertexBuffer.h"
#include "../MemoryManager/AssetManager.h"

Model::Model() :
	mDirectory(),
	mSkeleton(nullptr),
	mHasAnimations(false)
{
}

Model::~Model()
{
	std::cout << "Deleted model: " << mDirectory << "\n";

	mMeshes.clear();

	mMaterialMap.clear();

	delete mSkeleton;
}

void Model::MakeInstance(unsigned int numInstances)
{
	for (auto m : mMeshes)
	{
		m->GetVertexBuffer()->MakeInstance(numInstances);
	}
}

void Model::Draw(const glm::mat4& model)
{
	for (auto m : mMeshes)
	{
		m->Draw(model);
	}
}

void Model::Draw(Shader* s, const glm::mat4& model)
{
	for (auto m : mMeshes)
	{
		m->Draw(s, model);
	}
}
