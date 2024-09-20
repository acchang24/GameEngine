#pragma once
#include "SpriteComponent.h"

// BackgroundSprite component is used for a simple background in a 2D game
class BackgroundSpriteComponent :public SpriteComponent
{
public:
	// @param - Entity* to save a pointer to the component's entity
	// @param - Renderer2D* for the renderer
	// @param - const std::string& for the sprite texture's file name
	// @param - int for the draw order
	BackgroundSpriteComponent(Entity* owner, Renderer3D* renderer, const std::string& filename, int drawOrder = 100);
	~BackgroundSpriteComponent();

	// Override draw function that draws a backgtound sprite to the screen
	// @param - SDL_Renderer* for the renderer
	// @param - Entity2D* for the owner
	void Draw(Renderer3D* renderer, Entity2D* owner) override;

private:
};