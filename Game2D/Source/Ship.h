#pragma once
#include "Entity/Entity2D.h"

class CollisionComponent;
class Game;
class MoveComponent2D;
class SpriteComponent;
class SpriteRenderer;

class Ship : public Entity2D
{
public:
	Ship(SpriteRenderer* renderer, Game* game, int test);
	~Ship();

	// Override for ship's process input
	void OnProcessInput(const Uint8* keyState, const Keyboard* keyboard, const Mouse* mouse) override;

	// Override for ship's update
	void OnUpdate(float deltaTime) override;

private:
	SpriteComponent* mSprite;
	
	MoveComponent2D* mMovement;

	CollisionComponent* mCollisionBox;

	SpriteRenderer* mRenderer;

	Game* mGame;

	float mLaserCooldown;
};
