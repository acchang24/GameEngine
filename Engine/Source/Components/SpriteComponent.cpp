#include "SpriteComponent.h"
#include <iostream>
#include <glm/glm.hpp>
#include "../Entity/Entity.h"
#include "../Entity/Entity2D.h"
#include "../Graphics/Renderer3D.h"

SpriteComponent::SpriteComponent(Entity* owner, Renderer3D* renderer, const std::string& filename, int drawOrder) :
	Component(owner),
	mTexture(nullptr),
	mDrawOrder(drawOrder),
	mWidth(0),
	mHeight(0),
	mIsVisible(true)
{
	//SDL_Texture* spriteTexture = renderer->LoadSpriteTexture(filename);

	//SetTexture(spriteTexture);

	//renderer->AddSprite(this);
}

SpriteComponent::~SpriteComponent()
{
	std::cout << "Delete SpriteComponent\n";
}

void SpriteComponent::Draw(Renderer3D* renderer, Entity2D* owner)
{
	//if (mTexture)
	//{
	//	SDL_Rect r{};
	//	r.w = static_cast<int>(mWidth * owner->GetScale().x);
	//	r.h = static_cast<int>(mHeight * owner->GetScale().y);
	//	// Center the rectangle around the position of the owner
	//	r.x = static_cast<int>(owner->GetPosition().x - static_cast<float>(r.w / 2));
	//	r.y = static_cast<int>(owner->GetPosition().y - static_cast<float>(r.h / 2));

	//	// Draw (have to convert angle from radians to degrees, and clockwise to counter)
	//	SDL_RenderCopyEx(
	//		renderer->GetSdlRenderer(),
	//		mTexture,
	//		nullptr,
	//		&r,
	//		-glm::degrees(mOwner->GetRotation()),
	//		nullptr,
	//		SDL_FLIP_NONE
	//	);
	//}
}

void SpriteComponent::SetTexture(SDL_Texture* texture)
{
	mTexture = texture;

	// Set width/height
	SDL_QueryTexture(texture, nullptr, nullptr, &mWidth, &mHeight);
}
