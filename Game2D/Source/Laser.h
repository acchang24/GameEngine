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
	// @param - float for delta time
	// @param - const EngineContext& for the engine context
	void OnUpdate(float deltaTime, const EngineContext& engineContext) override;

private:
	SpriteComponent* mLaserSprite;
	MoveComponent2D* mLaserMovement;
	OBBComponent2D* mBox;
	Engine* mEngine;
	
	// lifetime tracker
	float mLaserDecay;
};
