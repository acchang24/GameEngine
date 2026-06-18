#include "Entity3D.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include "../Components/AnimationComponent3D.h"
#include "../Graphics/Model.h"
#include "../Graphics/Shader.h"

Entity3D::Entity3D() :
	Entity(),
	mUpdateModelMatrixJob(this),
	mModelMatrix(glm::mat4(1.0f)),
	mModel(nullptr),
	mInstanceBuffer(0)
{
}

Entity3D::~Entity3D()
{
	std::cout << "Deleted entity 3D\n";

	glDeleteBuffers(1, &mInstanceBuffer);
}

void Entity3D::MakeInstance(unsigned int numInstances, const void* data)
{
	glGenBuffers(1, &mInstanceBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mInstanceBuffer);
	glBufferData(GL_ARRAY_BUFFER, numInstances * sizeof(glm::mat4), data, GL_STATIC_DRAW);

	mModel->MakeInstance(numInstances);
}

void Entity3D::ProcessInput(const InputSystem* input, const EngineContext& engineContext)
{
	Entity::ProcessInput(input, engineContext);

	OnProcessInput(input, engineContext);
}

void Entity3D::Update(float deltaTime, const EngineContext& engineContext)
{
	Entity::Update(deltaTime, engineContext);

	if (mInstanceBuffer == 0)
	{
		OnUpdate(deltaTime, engineContext);
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

void Entity3D::OnProcessInput(const InputSystem* input, const EngineContext& engineContext)
{
}

void Entity3D::OnUpdate(float deltaTime, const EngineContext& engineContext)
{
	// Update model matrix on seprate thread		
	engineContext.jobManager->AddJob(&mUpdateModelMatrixJob);
	
	//CalculateWorldTransform();
}

void Entity3D::OnDraw()
{
	if (mModel->HasAnimations())
	{
		GetComponent<AnimationComponent3D>()->UpdateSkeletonBuffer();
	}

	mModel->Draw(mModelMatrix);
}

void Entity3D::OnDraw(Shader* shader)
{
	if (mModel->HasAnimations())
	{
		GetComponent<AnimationComponent3D>()->UpdateSkeletonBuffer();
	}

	shader->SetActive();
	shader->SetBool("isSkinned", mModel->HasAnimations());

	mModel->Draw(shader, mModelMatrix);
}

void Entity3D::CalculateWorldTransform()
{
	glm::mat4 model = glm::mat4(1.0f);

	model = glm::translate(model, mPosition);

	model = model * glm::mat4_cast(mRotation);

	mModelMatrix = glm::scale(model, mScale);
}

void Entity3D::UpdateModelMatrixJob::DoJob()
{
	mEntity->CalculateWorldTransform();
}
