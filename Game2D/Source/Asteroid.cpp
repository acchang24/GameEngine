#include "Asteroid.h"
#include "Util/Logger.h"
#include "Engine.h"
#include "Game.h"

Asteroid::Asteroid() :
	Entity()
{
}

Asteroid::~Asteroid()
{
	LOG_DEBUG("Destroyed Asteroid");
}

void Asteroid::OnUpdate(float deltaTime, const EngineContext& engineContext)
{
	// Wrap the screen if asteroid goes out of bounds
	if (mPosition.x < 0.0f)
	{
		mPosition.x = engineContext.renderer->GetWidth();
	}
	if (mPosition.x > engineContext.renderer->GetWidth())
	{
		mPosition.x = 0.0f;
	}
	if (mPosition.y < 0.0f)
	{
		mPosition.y = engineContext.renderer->GetHeight();
	}
	if (mPosition.y > engineContext.renderer->GetHeight())
	{
		mPosition.y = 0.0f;
	}
}

