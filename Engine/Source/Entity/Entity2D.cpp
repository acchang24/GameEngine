#include "Entity2D.h"
#include <iostream>
#include "../Components/SpriteComponent.h"

Entity2D::Entity2D(SDL_Renderer* renderer) :
	Entity(),
	mRenderer(renderer),
	mSprite(nullptr),
	mPosition(glm::vec2(0.0f, 0.0f)),
	mScale(glm::vec2(0.0f, 0.0f))
{
}

Entity2D::~Entity2D()
{
	std::cout << "Deleted Entity2D\n";
}

void Entity2D::ProcessInput(const Uint8* keyState)
{
	Entity::ProcessInput(keyState);

	OnProcessInput(keyState);
}

void Entity2D::OnProcessInput(const Uint8* keyState)
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

void Entity2D::Draw()
{
	if (mSprite)
	{
		mSprite->Draw(mRenderer, this);
	}
}
