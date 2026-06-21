#pragma once
#include "Entity/Entity.h"
#include "EngineContext.h"

class OBBComponent2D;
class Engine;
class MoveComponent2D;
class SpriteComponent;

class Laser : public Entity
{
public:
	Laser(const EngineContext& engineContext);
	~Laser();

	// OnUpdate override
	// @param - float for delta time
	// @param - const EngineContext& for the engine context
	void OnUpdate(float deltaTime, const EngineContext& engineContext) override;

private:
	SpriteComponent* mLaserSprite;
	MoveComponent2D* mLaserMovement;
	OBBComponent2D* mBox;
	
	// lifetime tracker
	float mLaserDecay;
};
