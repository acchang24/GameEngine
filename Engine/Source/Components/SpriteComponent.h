#pragma once
#include "Component.h"
#include <string>
#include <unordered_map>

class Renderer2D;
class Texture;

class SpriteComponent : public Component
{
public:
	// SpriteComponent constructor:
	// @param - Entity* for the owner
	// @param - SpriteRenderer* for the sprite renderer
	// @param - int for optional draw order. (lower number means it will be drawn further back)
	SpriteComponent(Entity* owner, Renderer2D* renderer, int drawOrder = 100);
	~SpriteComponent();

	// Adds a sprite to the map of sprites
	// @param - Texture* for the sprite
	void AddSprite(Texture* sprite);

	// Gets a sprite to the map by file name. Returns nullptr if not found
	// @param - const std::string& for the sprite's file name
	// @return - Texture* for the sprite
	Texture* GetSprite(const std::string& spriteName);

	// Gets the current sprite being drawn
	// @return - Texture* for the current sprite
	Texture* GetCurrentSprite() { return mCurrentSprite; }

	// Gets the sprite's draw order
	// @return - int for the draw order
	int GetDrawOrder() const { return mDrawOrder; }

	// Sees if this sprite is visible
	// @return - bool for if the sprite is visible or not
	bool IsVisible() const { return mIsVisible; }

	// Sets the current sprite to draw
	// @param - Texture* for the new sprite
	void SetSprite(Texture* sprite) { mCurrentSprite = sprite; }

	// Sets the visiblity of a sprite
	// @param - bool for if the sprite is visible or not
	void SetIsVisible(bool visible) { mIsVisible = visible; }

private:
	// Map of sprites/textures used by the owner
	std::unordered_map<std::string, Texture*> mSprites;

	// Pointer to the sprite renderer
	Renderer2D* mRenderer;

	// Current sprite
	Texture* mCurrentSprite;

	// Draw order for the sprite. (lower number means further back)
	int mDrawOrder;

	// Bool for if a texture is visible
	bool mIsVisible;
};
