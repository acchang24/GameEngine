#pragma once
#include "Entity/Entity2D.h"

class CollisionComponent;
class Engine;
class Game;
class MoveComponent2D;
class SpriteComponent;

class Asteroid :public Entity2D
{
public:
	Asteroid(Game* game);
	~Asteroid();

	// Override for asteroid's update
	void OnUpdate(float deltaTime) override;

private:
	SpriteComponent* mSprite;

	MoveComponent2D* mMovement;

	CollisionComponent* mCollisionCircle;

	Engine* mEngine;
};