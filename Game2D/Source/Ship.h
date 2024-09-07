#pragma once
#include "Entity/Entity2D.h"

class Renderer2D;
class SpriteComponent;

class Ship : public Entity2D
{
public:
	Ship(Renderer2D* renderer);

	void OnProcessInput(const Uint8* keyState) override;

	void OnUpdate(float deltaTime) override;

private:
	SpriteComponent* mShipSprite;


};