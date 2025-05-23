#include "Entity3D.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../Animation/Skeleton.h"
#include "../Components/AnimationComponent.h"
#include "../Graphics/Model.h"
#include "../Graphics/Shader.h"
#include "../MemoryManager/AssetManager.h"

Entity3D::Entity3D() :
	Entity(),
	mUpdateModelMatrixJob(this),
	mModelMatrix(glm::mat4(1.0f)),
	mQuatRotation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f)),
	mPosition(glm::vec3(0.0f, 0.0f, 0.0f)),
	mScale(glm::vec3(1.0f, 1.0f, 1.0f)),
	mModel(nullptr),
	mInstanceBuffer(0),
	mYaw(0.0f),
	mPitch(0.0f),
	mRoll(0.0f),
	mIsSkinned(false)
{
}

Entity3D::Entity3D(const std::string& fileName):
	Entity(),
	mUpdateModelMatrixJob(this),
	mModelMatrix(glm::mat4(1.0f)),
	mQuatRotation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f)),
	mPosition(glm::vec3(0.0f, 0.0f, 0.0f)),
	mScale(glm::vec3(1.0f, 1.0f, 1.0f)),
	mModel(nullptr),
	mInstanceBuffer(0),
	mYaw(0.0f),
	mPitch(0.0f),
	mRoll(0.0f),
	mIsSkinned(false)
{
	Model* model = AssetManager::LoadModel(fileName);

	if (model)
	{
		// Copy new model
		mModel = model;

		// Check if it has a skeleton
		if (model->HasSkeleton())
		{
			mIsSkinned = true;
			// Copy new skeleton and set create an animation component for this entity
			Skeleton* newSkeleton = new Skeleton(*AssetManager::LoadSkeleton(fileName));
			AnimationComponent* animComp = new AnimationComponent(this, newSkeleton);
		}
	}
	else
	{
		mModel = new Model(fileName, this);
	}
}

Entity3D::~Entity3D()
{
	glDeleteBuffers(1, &mInstanceBuffer);

	std::cout << "Deleted entity 3D\n";
}

void Entity3D::MakeInstance(unsigned int numInstances, const void* data)
{
	glGenBuffers(1, &mInstanceBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mInstanceBuffer);
	glBufferData(GL_ARRAY_BUFFER, numInstances * sizeof(glm::mat4), data, GL_STATIC_DRAW);

	mModel->MakeInstance(numInstances);
}

void Entity3D::ProcessInput(const Uint8* keyState, const Keyboard* keyboard, const Mouse* mouse)
{
	Entity::ProcessInput(keyState, keyboard, mouse);

	OnProcessInput(keyState, keyboard, mouse);
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

void Entity3D::OnProcessInput(const Uint8* keyState, const Keyboard* keyboard, const Mouse* mouse)
{
}

void Entity3D::OnUpdate(float deltaTime)
{
	// Update model matrix on seprate thread		
	JobManager::Get()->AddJob(&mUpdateModelMatrixJob);
	
	//CalculateWorldTransform();
}

void Entity3D::OnDraw()
{
	if (mIsSkinned)
	{
		GetComponent<AnimationComponent>()->UpdateSkeletonBuffer();
	}

	mModel->Draw(mModelMatrix);
}

void Entity3D::OnDraw(Shader* shader)
{
	if (mIsSkinned)
	{
		GetComponent<AnimationComponent>()->UpdateSkeletonBuffer();
	}

	shader->SetActive();
	shader->SetBool("isSkinned", mIsSkinned);

	mModel->Draw(shader, mModelMatrix);
}

void Entity3D::CalculateWorldTransform()
{
	glm::mat4 model = glm::mat4(1.0f);

	glm::mat4 translate = glm::translate(model, mPosition);

	glm::mat4 rotation = glm::toMat4(mQuatRotation);

	// Euler angles
	//glm::mat4 rotation = glm::rotate(model, glm::radians(mRoll), glm::vec3(0.0f, 0.0f, 1.0f));
	//rotation = glm::rotate(rotation, glm::radians(mPitch), glm::vec3(1.0f, 0.0f, 0.0f));
	//rotation = glm::rotate(rotation, glm::radians(mYaw), glm::vec3(0.0f, 1.0f, 0.0f));

	glm::mat4 scale = glm::scale(model, mScale);

	mModelMatrix = translate * rotation * scale;
}

void Entity3D::UpdateModelMatrixJob::DoJob()
{
	mEntity->CalculateWorldTransform();
}
