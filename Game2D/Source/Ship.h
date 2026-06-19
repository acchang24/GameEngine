#pragma once
#include "Entity/Entity.h"

class CollisionComponent;
class Engine;
class Game;
class MoveComponent2D;
class SpriteComponent;

class Ship : public Entity
{
public:
	Ship(Game* game);
	~Ship();

	// Override for ship's process input
	// @param - const InputSystem* for the input system
	// @param - const EngineContext& for the engine context
	void OnProcessInput(const InputSystem* input, const EngineContext& engineContext) override;

	// Override for ship's update
	// @param - float for delta time
	// @param - const EngineContext& for the engine context
	void OnUpdate(float deltaTime, const EngineContext& engineContext) override;

	void SetSpriteComp(SpriteComponent* comp) { mSprite = comp; }
	void SetMoveComp(MoveComponent2D* comp) { mMovement = comp; }
	void SetCollisionComp(CollisionComponent* comp) { mCollisionBox = comp; }

private:
	SpriteComponent* mSprite;
	
	MoveComponent2D* mMovement;

	CollisionComponent* mCollisionBox;

	Engine* mEngine;

	Game* mGame;

	float mLaserCooldown;
};
