#pragma once
#include "Entity/Entity2D.h"

class OBBComponent2D;
class Game;
class MoveComponent2D;
class SpriteComponent;
class SpriteRenderer;

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
	OBBComponent2D* mBox;
	Game* mGame;
	
	// lifetime tracker
	float mLaserDecay;
};
