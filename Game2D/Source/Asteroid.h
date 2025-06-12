#pragma once
#include "Entity/Entity2D.h"

class CollisionComponent;
class Game;
class MoveComponent2D;
class SpriteComponent;
class SpriteRenderer;

class Asteroid :public Entity2D
{
public:
	Asteroid(SpriteRenderer* renderer, Game* game);
	~Asteroid();

	// Override for asteroid's update
	void OnUpdate(float deltaTime) override;

private:
	SpriteComponent* mSprite;

	MoveComponent2D* mMovement;

	CollisionComponent* mCollisionCircle;

	SpriteRenderer* mRenderer;
};