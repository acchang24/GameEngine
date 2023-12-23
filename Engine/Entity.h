#pragma once
#include <vector>
#include <glm/glm.hpp>

class Component;

// Entity class represents a general-purpose object within a game.
// Each entity contains functions to help update and render on each frame.
// Unique entities for specific game objects are intended to inherit from this class.
// Entities contains an array of components that are associated with it.
class Entity
{
public:
	Entity();
	virtual ~Entity();

	// Virtual Update function that updates and handles the entity's own unique attributes.
	// This Update function loops through the vector of components updates them.
	// @param - float representing delta time
	virtual void Update(float deltaTime);

	// Virtual Draw function that draws/presents the object to the screen
	virtual void Draw();

	// Adds a component to mComponents
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

protected:
	// Vector of components the entity uses
	std::vector<Component*> mComponents;
};