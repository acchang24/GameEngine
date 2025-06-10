#pragma once

#include "Entity/Entity2D.h"

class Game;
class CircleComponent;
class MoveComponent2D;
class SpriteComponent;
class SpriteRenderer;
class Asteroid :public Entity2D
{
public:
	Asteroid(SpriteRenderer* renderer, Game* game);
	~Asteroid();

	// Override for ship's update
	void OnUpdate(float deltaTime) override;

private:
	MoveComponent2D* mMovement;

	SpriteComponent* mSprite;

	CircleComponent* mCollisionCircle;

	SpriteRenderer* mRenderer;
};