#pragma once
#include "Entity/Entity2D.h"

class Renderer3D;
class MoveComponent;
class SpriteComponent;

class Ship : public Entity2D
{
public:
	Ship(Renderer3D* renderer);

	void OnProcessInput(const Uint8* keyState) override;

	void OnUpdate(float deltaTime) override;

private:
	MoveComponent* mMove;

	SpriteComponent* mShipSprite;
};
