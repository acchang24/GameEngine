#pragma once
#include <SDL2/SDL_stdinc.h>

class Entity;

// The Component class defines a particular aspect or functionality
// that an entity possesses. The component is stored in the entity's
// vector of components, and when combined, helps define reusable 
// functionality to a game entity such as movement, collision, AI, etc.
class Component
{
public:
	// Component constructor adds this component to the owner's array of components
	// @param - Entity* to save a pointer to the component's entity
	Component(Entity* owner);
	virtual ~Component();

	// Overridable process input function for this component if it's needed
	// @param - const Uint8* for the keyboard state
	virtual void ProcessInput(const Uint8* keyState);

	// Overridable update function that updates a particular component
	// @param - float for delta time
	virtual void Update(float deltaTime);

	// Getter for entity
	// @return - Entity* for the component's entity
	Entity* GetEntity() const { return mOwner; }

protected:
	// The entity/owner that uses this component
	Entity* mOwner;
};
