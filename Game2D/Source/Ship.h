#pragma once
#include "Entity/Entity2D.h"

class Game;
class SpriteComponent;
class SpriteRenderer;

class Ship : public Entity2D
{
public:
	Ship(SpriteRenderer* renderer, Game* game);

	// Override for ship's process input
	void OnProcessInput(const Uint8* keyState, const Keyboard* keyboard, const Mouse* mouse) override;

	// Override for ship's update
	void OnUpdate(float deltaTime) override;

private:
	SpriteComponent* mSpriteComponent;
};
