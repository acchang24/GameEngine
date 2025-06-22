#pragma once
#include "Entity/Entity2D.h"

class OBBComponent2D;
class Engine;
class Game;
class MoveComponent2D;
class SpriteComponent;

class Laser : public Entity2D
{
public:
	Laser(Game* game);
	~Laser();

	// OnUpdate override
	void OnUpdate(float deltaTime) override;

private:
	SpriteComponent* mLaserSprite;
	MoveComponent2D* mLaserMovement;
	OBBComponent2D* mBox;
	Engine* mEngine;
	
	// lifetime tracker
	float mLaserDecay;
};
