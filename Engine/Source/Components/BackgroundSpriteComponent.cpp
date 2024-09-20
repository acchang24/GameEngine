#include "BackgroundSpriteComponent.h"
#include "../Graphics/Renderer2D.h"
#include "../Entity/Entity2D.h"

BackgroundSpriteComponent::BackgroundSpriteComponent(Entity* owner, Renderer3D* renderer, const std::string& filename, int drawOrder) :
	SpriteComponent(owner, renderer, filename, drawOrder)
{
}

BackgroundSpriteComponent::~BackgroundSpriteComponent()
{
}

void BackgroundSpriteComponent::Draw(Renderer3D* renderer, Entity2D* owner)
{
	//if (mTexture)
	//{
	//	SDL_Rect r{};
	//	// Stretch background to the renderer's width and height
	//	r.w = static_cast<int>(renderer->GetWidth() * owner->GetScale().x);
	//	r.h = static_cast<int>(renderer->GetHeight() * owner->GetScale().y);
	//	// Center the rectangle around 0
	//	r.x = 0;
	//	r.y = 0;

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
