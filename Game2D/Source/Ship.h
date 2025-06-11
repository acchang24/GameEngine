#pragma once
#include "Entity/Entity2D.h"

class AABBComponent2D;
class Game;
class MoveComponent2D;
class SpriteComponent;
class SpriteRenderer;

class Ship : public Entity2D
{
public:
	Ship(SpriteRenderer* renderer, Game* game);
	~Ship();

	// Override for ship's process input
	void OnProcessInput(const Uint8* keyState, const Keyboard* keyboard, const Mouse* mouse) override;

	// Override for ship's update
	void OnUpdate(float deltaTime) override;

private:
	SpriteComponent* mSprite;
	
	MoveComponent2D* mMovement;

	AABBComponent2D* mCollisionBox;

	SpriteRenderer* mRenderer;

	Game* mGame;

	float mLaserCooldown;
};
