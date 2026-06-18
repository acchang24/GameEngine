#pragma once
#include "Entity/Entity.h"

class CollisionComponent;
class Engine;
class Game;
class MoveComponent2D;
class SpriteComponent;

class Asteroid :public Entity
{
public:
	Asteroid(Game* game);
	~Asteroid();

	// Override for asteroid's update
	// @param - float for delta time
	// @param - const EngineContext& for the engine context
	void OnUpdate(float deltaTime, const EngineContext& engineContext) override;

private:
	SpriteComponent* mSprite;

	MoveComponent2D* mMovement;

	CollisionComponent* mCollisionCircle;

	Engine* mEngine;
};