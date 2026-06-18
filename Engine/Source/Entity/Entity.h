#pragma once
#include <cmath>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "../EngineContext.h"

enum class EntityState
{
	Active,
	Paused,
	Destroy
};

class Component;
class InputSystem;

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
	// @param - const InputSystem* for the input system
	// @param - const EngineContext& for the engine context
	virtual void ProcessInput(const InputSystem* input, const EngineContext& engineContext);

	// Virtual Update function that updates and handles the entity's own unique attributes.
	// This base/parent class Update function simply loops through the vector of components updates them.
	// @param - float representing delta time
	// @param - const EngineContext& for the engine context
	virtual void Update(float deltaTime, const EngineContext& engineContext);

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

	// Returns the entity's 2D rotation angle in radians
	// @return - float for rotation in radians
	float GetRotation2D() const { return 2.0f * std::atan2(mRotation.z, mRotation.w); }

	// Gets the entity's rotation as a quaternion
	// @return - const glm::quat& for the rotation
	const glm::quat& GetQuatRotation() const { return mRotation; }

	// Sets the rotation for a 2D entity
	// @param - const glm::quat& for the rotation
	void SetRotation2D(const glm::quat& rot) { mRotation = glm::normalize(rot); }

	// Gets the forward facing direction of a 2D entity
	// return - const glm::vec2 for the forward direction
	const glm::vec2 GetForward2D() const
	{
		float rot = GetRotation2D();
		return glm::vec2(glm::cos(rot), glm::sin(rot));
	}

	// Gets the entity's state
	// @return - EntityState for the entity's state
	EntityState GetEntityState() const { return mState; }

	// Sets the entity's state
	// @param - EntityState for the entity's new state
	void SetEntityState(EntityState state) { mState = state; }

protected:
	// Vector of components the entity uses
	std::vector<Component*> mComponents;

	// Entity's rotation
	glm::quat mRotation;

	// Entity's state
	EntityState mState;
};
