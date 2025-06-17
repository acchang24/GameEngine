#pragma once
#include "Entity/Entity2D.h"

class CollisionComponent;
class Engine;
class Game;
class MoveComponent2D;
class SpriteComponent;
class Renderer2D;

class Ship : public Entity2D
{
public:
	Ship(Renderer2D* renderer, Game* game);
	~Ship();

	// Override for ship's process input
	void OnProcessInput(const Uint8* keyState, Keyboard* keyboard, const Mouse* mouse) override;

	// Override for ship's update
	void OnUpdate(float deltaTime) override;

private:
	SpriteComponent* mSprite;
	
	MoveComponent2D* mMovement;

	CollisionComponent* mCollisionBox;

	Renderer2D* mRenderer;

	Engine* mEngine;

	Game* mGame;

	float mLaserCooldown;
};
