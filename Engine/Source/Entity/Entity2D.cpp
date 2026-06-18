#include "Entity2D.h"
#include <iostream>

Entity2D::Entity2D() :
	Entity(),
	mPosition(glm::vec2(0.0f, 0.0f)),
	mScale(1.0f)
{
}

Entity2D::Entity2D(float width, float height) :
	Entity(),
	mPosition(glm::vec2(0.0f, 0.0f)),
	mScale(1.0f)
{
}

Entity2D::~Entity2D()
{
	std::cout << "Deleted Entity2D\n";
}

void Entity2D::ProcessInput(const InputSystem* input, const EngineContext& engineContext)
{
	Entity::ProcessInput(input, engineContext);

	OnProcessInput(input, engineContext);
}

void Entity2D::OnProcessInput(const InputSystem* input, const EngineContext& engineContext)
{
}

void Entity2D::Update(float deltaTime, const EngineContext& engineContext)
{
	Entity::Update(deltaTime, engineContext);

	OnUpdate(deltaTime, engineContext);
}

void Entity2D::OnUpdate(float deltaTime, const EngineContext& engineContext)
{
}
