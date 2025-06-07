#include "SpriteComponent.h"
#include <iostream>
#include "../Graphics/SpriteRenderer.h"
#include "../Graphics/Texture.h"

SpriteComponent::SpriteComponent(Entity* owner, SpriteRenderer* renderer, int drawOrder) :
	Component(owner),
	mRenderer(renderer),
	mCurrentSprite(nullptr),
	mDrawOrder(drawOrder),
	mIsVisible(true)
{
	mRenderer->AddSprite(this);
}

SpriteComponent::~SpriteComponent()
{
	std::cout << "Delete SpriteComponent\n";

	mRenderer->RemoveSprite(this);
}

void SpriteComponent::AddSprite(Texture* sprite)
{
	std::string spriteName = sprite->GetName();
	auto iter = mSprites.find(spriteName);
	if (iter == mSprites.end())
	{
		mSprites[spriteName] = sprite;
	}
	else
	{
		std::cout << "Sprite name: " << spriteName << " already exists\n";
	}
}

Texture* SpriteComponent::GetSprite(const std::string& spriteName)
{
	auto iter = mSprites.find(spriteName);
	if (iter != mSprites.end())
	{
		return iter->second;
	}
	return nullptr;
}
