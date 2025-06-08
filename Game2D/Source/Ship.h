#pragma once
#include "Entity/Entity2D.h"

class Game;
class AABBComponent2D;
class MoveComponent2D;
class SpriteComponent;
class SpriteRenderer;

class Ship : public Entity2D
{
public:
	Ship(SpriteRenderer* renderer, Game* game, int test);

	// Override for ship's process input
	void OnProcessInput(const Uint8* keyState, const Keyboard* keyboard, const Mouse* mouse) override;

	// Override for ship's update
	void OnUpdate(float deltaTime) override;

	Game* GetGame() { return mGame; }

private:
	MoveComponent2D* mMovement;

	SpriteComponent* mSprite;

	AABBComponent2D* mCollisionBox;

	SpriteRenderer* mRenderer;

	Game* mGame;
};
