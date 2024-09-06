#include "SpriteComponent.h"
#include "../Entity/Entity.h"

SpriteComponent::SpriteComponent(Entity* owner, int drawOrder) :
	Component(owner),
	mTexture(nullptr),
	mDrawOrder(drawOrder),
	mWidth(0),
	mHeight(0),
	mIsVisible(true)
{

}

SpriteComponent::~SpriteComponent()
{

}

void SpriteComponent::Draw(SDL_Renderer* renderer)
{
	//if (mTexture)
	//{
	//	SDL_Rect r{};
	//	r.w = static_cast<int>(mWidth * mOwner->GetScale());
	//	r.h = static_cast<int>(mHeight * mOwner->GetScale());
	//	// Center the rectangle around the position of the owner
	//	r.x = static_cast<int>(mOwner->GetPosition().x - r.w / 2);
	//	r.y = static_cast<int>(mOwner->GetPosition().y - r.h / 2);

	//	// Draw (have to convert angle from radians to degrees, and clockwise to counter)
	//	SDL_RenderCopyEx(renderer,
	//		mTexture,
	//		nullptr,
	//		&r,
	//		-Math::ToDegrees(mOwner->GetRotation()),
	//		nullptr,
	//		SDL_FLIP_NONE);
	//}
}

void SpriteComponent::SetTexture(SDL_Texture* texture)
{
	mTexture = texture;

	// Set width/height
	SDL_QueryTexture(texture, nullptr, nullptr, &mWidth, &mHeight);
}
