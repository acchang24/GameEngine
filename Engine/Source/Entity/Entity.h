#pragma once
#include <vector>
#include <SDL2/SDL_stdinc.h>

enum class EntityState
{
	Active,
	Paused,
	Destroy
};

class Component;
class Keyboard;
class Mouse;

// Entity class represents a general object within a game.
// Each entity contains functions to help update and render on each frame.
// Unique entities for specific game objects are intended to inherit from this class.
// Entities contains an array of components that are associated with it.
class Entity
{
public:
	Entity();
	virtual ~Entity();

	// Virtual ProcessInput that checks if the entity is active.
	// If so, it will Call ProcessInput on all of its components
	// @param - const Uint8* for the keyboard state
	// @param - const Keyboard* for the keyboard
	// @param - const Mouse* for the mouse
	virtual void ProcessInput(const Uint8* keyState, const Keyboard* keyboard, const Mouse* mouse);

	// Virtual Update function that updates and handles the entity's own unique attributes.
	// This base/parent class Update function simply loops through the vector of components updates them.
	// @param - float representing delta time
	virtual void Update(float deltaTime);

	// Virtual Draw function that draws/presents the object to the screen
	virtual void Draw();

	// Adds a component to the entity's vector of components
	void AddComponent(Component* c) { mComponents.emplace_back(c); }

	// Returns a component of type T, or null if it doesn't exist
	template <typename T>
	T* GetComponent() const
	{
		// Loop through each component of the component vector
		// and cast to see if it matches type T*
		for (auto c : mComponents)
		{
			T* t = dynamic_cast<T*>(c);
			if (t != nullptr)
			{
				return t;
			}
		}
		return nullptr;
	}

	// Gets the entity's state
	// @return - EntityState for the entity's state
	EntityState GetEntityState() const { return mState; }

	// Gets the entity's rotation as a float value
	// @return - float for the rotation
	float GetRotation() const { return mRotation; }

	// Sets the entity's state
	// @param - EntityState for the entity's new state
	void SetEntityState(EntityState state) { mState = state; }

	// Sets the entity's single float value rotation
	// @param - float for the new rotation
	void SetRotation(float rotation) { mRotation = rotation; }

protected:
	// Vector of components the entity uses
	std::vector<Component*> mComponents;

	// Entity's state
	EntityState mState;

	// Entity's rotation in radians as a float
	float mRotation;
};
