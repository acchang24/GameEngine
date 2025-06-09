#pragma once
#include "Entity/Entity2D.h"

class Game;
class SpriteRenderer;
class SpriteComponent;
class MoveComponent2D;
class AABBComponent2D;

class Laser : public Entity2D
{
public:
	Laser(SpriteRenderer* renderer, Game* game);
	~Laser();

	// OnUpdate override
	void OnUpdate(float deltaTime) override;


private:
	SpriteComponent* mLaserSprite;
	MoveComponent2D* mLaserMovement;
	SpriteRenderer* mRenderer;
	AABBComponent2D* mBox;

	Game* mGame;

	// lifetime tracker
	float mLaserDecay;
};