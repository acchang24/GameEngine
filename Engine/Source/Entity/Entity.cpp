#include "Entity.h"
#include <iostream>
#include "../Components/Component.h"
#include "../Input/InputSystem.h"

Entity::Entity() :
	mModelMatrix(glm::mat4(1.0f)),
	mPosition(glm::vec3()),
	mRotation(glm::quat()),
	mScale(glm::vec3(1.0f, 1.0f, 1.0f)),
	mModel(nullptr),
	mState(EntityState::Active)
{

}

Entity::~Entity()
{
	std::cout << "Deleted entity\n";

	for (auto c : mComponents)
	{
		delete c;
	}
	mComponents.clear();
}

void Entity::ProcessInput(const InputSystem* input, const EngineContext& engineContext)
{
	if (mState == EntityState::Active)
	{
		for (auto c : mComponents)
		{
			c->ProcessInput(input, engineContext);
		}
	}
	OnProcessInput(input, engineContext);
}

void Entity::OnProcessInput(const InputSystem* input, const EngineContext& engineContext)
{
}

void Entity::Update(float deltaTime, const EngineContext& engineContext)
{
	if (mState == EntityState::Active)
	{
		for (auto c : mComponents)
		{
			c->Update(deltaTime, engineContext);
		}
	}
	OnUpdate(deltaTime, engineContext);
}

void Entity::OnUpdate(float deltaTime, const EngineContext& engineContext)
{
}

void Entity::Draw()
{

}

const glm::mat4& Entity::GetModelMatrix()
{
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, mPosition);
	model = model * glm::mat4_cast(mRotation);
	mModelMatrix = glm::scale(model, mScale);

	return mModelMatrix;
}
