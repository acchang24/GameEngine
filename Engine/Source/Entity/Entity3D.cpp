#include "Entity3D.h"
#include <iostream>
#include <queue>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include "../Graphics/Mesh.h"
#include "../Graphics/VertexLayouts.h"
#include "../Graphics/VertexBuffer.h"
#include "../MemoryManager/AssetManager.h"
#include "../Animation/Skeleton.h"
#include "../Animation/Animation.h"
#include "../Graphics/UniformBuffer.h"
#include "../Profiler/Profiler.h"
#include "../Components/AnimationComponent.h"
#include "../Graphics/Model.h"

Entity3D::Entity3D() :
	Entity(),
	mModel(nullptr),
	mUpdateModelMatrixJob(this),
	mModelMatrix(glm::mat4(1.0f)),
	mPosition(glm::vec3(0.0f, 0.0f, 0.0f)),
	mScale(glm::vec3(1.0f, 1.0f, 1.0f)),
	mInstanceBuffer(0),
	mYaw(0.0f),
	mPitch(0.0f),
	mRoll(0.0f),
	mIsSkinned(false)
{
}

Entity3D::Entity3D(const std::string& fileName):
	Entity(),
	mModel(nullptr),
	mUpdateModelMatrixJob(this),
	mModelMatrix(glm::mat4(1.0f)),
	mPosition(glm::vec3(0.0f, 0.0f, 0.0f)),
	mScale(glm::vec3(1.0f, 1.0f, 1.0f)),
	mInstanceBuffer(0),
	mYaw(0.0f),
	mPitch(0.0f),
	mRoll(0.0f),
	mIsSkinned(false)
{
	AssetManager* am = AssetManager::Get();

	Model* model = am->LoadModel(fileName);

	if (model)
	{
		AnimationComponent* animComp = new AnimationComponent(this, new Skeleton(*model->GetSkeleton()));
		mModel = model;
	}
	else
	{
		mModel = new Model(fileName, this);
	}
}

Entity3D::~Entity3D()
{
	std::cout << "Delete entity 3D" << std::endl;

	glDeleteBuffers(1, &mInstanceBuffer);
}

void Entity3D::MakeInstance(unsigned int numInstances, const void* data)
{
	glGenBuffers(1, &mInstanceBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mInstanceBuffer);
	glBufferData(GL_ARRAY_BUFFER, numInstances * sizeof(glm::mat4), data, GL_STATIC_DRAW);

	mModel->MakeInstance(numInstances);
}

void Entity3D::Update(float deltaTime)
{
	Entity::Update(deltaTime);

	if (mInstanceBuffer == 0)
	{
		OnUpdate(deltaTime);
	}
}

void Entity3D::Draw()
{
	OnDraw();
}

void Entity3D::Draw(Shader* shader)
{
	if (mInstanceBuffer == 0)
	{
		OnDraw(shader);
	}
}

void Entity3D::OnUpdate(float deltaTime)
{
	// Update model matrix on seprate thread		
	JobManager::Get()->AddJob(&mUpdateModelMatrixJob);
	
	//mModel = glm::mat4(1.0f);

	//// Translate
	//mModel = glm::translate(mModel, mPosition);

	//// Rotate
	//mModel = glm::rotate(mModel, glm::radians(mRoll), glm::vec3(0.0f, 0.0f, 1.0f));
	//mModel = glm::rotate(mModel, glm::radians(mPitch), glm::vec3(1.0f, 0.0f, 0.0f));
	//mModel = glm::rotate(mModel, glm::radians(mYaw), glm::vec3(0.0f, 1.0f, 0.0f));

	//// Scale
	//mModel = glm::scale(mModel, mScale);
}

void Entity3D::OnDraw()
{
	if (mIsSkinned)
	{
		GetComponent<AnimationComponent>()->UpdateBoneMatrices();
	}

	mModel->Draw(mModelMatrix);
}

void Entity3D::OnDraw(Shader* shader)
{
	if (mIsSkinned)
	{
		GetComponent<AnimationComponent>()->UpdateBoneMatrices();
	}

	mModel->Draw(shader, mModelMatrix);
}

void Entity3D::UpdateModelMatrixJob::DoIt()
{
	glm::mat4 model = glm::mat4(1.0f);

	// Translate
	model = glm::translate(model, mEntity->mPosition);

	// Rotate
	model = glm::rotate(model, glm::radians(mEntity->mRoll), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::rotate(model, glm::radians(mEntity->mPitch), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(mEntity->mYaw), glm::vec3(0.0f, 1.0f, 0.0f));

	// Scale
	model = glm::scale(model, mEntity->mScale);

	mEntity->mModelMatrix = model;
}
