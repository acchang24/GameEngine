#include "Entity2D.h"
#include <iostream>

Entity2D::Entity2D() :
	Entity(),
	mPosition(glm::vec2(0.0f, 0.0f)),
	mSize(glm::vec2(5.0f, 5.0f)),
	mRotation(0.0f),
	mScale(1.0f)
{
}

Entity2D::Entity2D(float width, float height) :
	Entity(),
	mPosition(glm::vec2(0.0f, 0.0f)),
	mSize(glm::vec2(width, height)),
	mRotation(0.0f),
	mScale(1.0f)
{
}

Entity2D::~Entity2D()
{
	std::cout << "Deleted Entity2D\n";
}

void Entity2D::ProcessInput(const Uint8* keyState, Keyboard* keyboard, const Mouse* mouse)
{
	Entity::ProcessInput(keyState, keyboard, mouse);

	OnProcessInput(keyState, keyboard, mouse);
}

void Entity2D::OnProcessInput(const Uint8* keyState, Keyboard* keyboard, const Mouse* mouse)
{
}

void Entity2D::Update(float deltaTime)
{
	Entity::Update(deltaTime);

	OnUpdate(deltaTime);
}

void Entity2D::OnUpdate(float deltaTime)
{
}
