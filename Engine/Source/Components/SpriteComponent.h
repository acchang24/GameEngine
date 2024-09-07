#pragma once
#include "Component.h"
#include <string>
#include <SDL2/SDL.h>

class Entity;
class Entity2D;
class Renderer2D;

class SpriteComponent : public Component 
{
public:
	// SpriteComponent constructor:
	// Creates an SDL sprite texture and adds it to the Renderer2D
	// map of sprites as well as its vector of sprites sorted by draw order
	// @param - Entity* to save a pointer to the component's entity
	// @param - Renderer2D* for the renderer
	// @param - const std::string& for the sprite texture's file name
	// @param - int for the draw order
	SpriteComponent(Entity* owner, Renderer2D* renderer, const std::string& filename, int drawOrder = 100);
	~SpriteComponent();

	// Overridable draw function that draws a sprite to the screen
	// @param - SDL_Renderer* for the renderer
	// @param - Entity2D* for the owner
	virtual void Draw(SDL_Renderer* renderer, Entity2D* owner);

	// Sets the texture to draw for this sprite
	virtual void SetTexture(SDL_Texture* texture);

	// Gets the draw order for this sprite
	// @return - int for the draw order
	int GetDrawOrder() const { return mDrawOrder; }

	// Gets the sprites's width
	// @return - int for the sprite's width
	int GetWidth() const { return mWidth; }

	// Gets the sprites's height
	// @return - int for the sprite's height
	int GetHeight() const { return mHeight; }

	// Sees if this sprite is visible
	// @return - bool for if the sprite is visible
	bool IsVisible() const { return mIsVisible; }

	// Sets the sprite's visiblity
	// @param - bool for if the sprite is visible
	void SetIsVisible(bool visible) { mIsVisible = visible; }

private:
	// SDL texture to draw
	SDL_Texture* mTexture;

	// Draw order determines which sprites will be drawn first
	int mDrawOrder;

	// Sprite width
	int mWidth;

	// Sprite height
	int mHeight;

	// If this sprite is visible
	bool mIsVisible;
};