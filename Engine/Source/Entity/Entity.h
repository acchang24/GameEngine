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
class Model;

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

	// Entity specific ProcessInput code (overridable)
	// @param - const InputSystem* for the input system
	// @param - const EngineContext& for the engine context
	virtual void OnProcessInput(const InputSystem* input, const EngineContext& engineContext);

	// Virtual Update function that updates and handles the entity's own unique attributes.
	// This base/parent class Update function simply loops through the vector of components updates them.
	// @param - float representing delta time
	// @param - const EngineContext& for the engine context
	virtual void Update(float deltaTime, const EngineContext& engineContext);

	// Entity specific update code (overridable)
	// @param - float for delta time
	// @param - const EngineContext& for the engine context
	virtual void OnUpdate(float deltaTime, const EngineContext& engineContext);

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

	// Calculates and returns the model matrix
	// @return - const glm::mat4& for the model matrix
	const glm::mat4& GetModelMatrix();

	// Returns the entity's 3D position
	// @return - const glm::vec3& for the position
	const glm::vec3& GetPosition3D() const { return mPosition; }

	// Returns the entity's 3D rotation as a quaternion
	// @return - const glm::quat& for the rotation
	const glm::quat& GetRotation3D() const { return mRotation; }

	// Returns the entity's 3D scale
	// @return - const glm::vec3& for the scale
	const glm::vec3& GetScale3D() const { return mScale; }

	// Sets the entity's 3D position
	// @param - const glm::vec3& for the position
	void SetPosition3D(const glm::vec3& pos) { mPosition = pos; }

	// Sets the entity's 3D Rotation using a quaternion
	// @param - const glm::quat& for the rotation
	void SetRotation3D(const glm::quat& rotation) { mRotation = rotation; }

	// Sets the entity's 3D scale using 3 float values
	// @param - float for width
	// @param - float for height
	// @param - float for depth
	void SetScale3D(float w, float h, float d) { mScale = glm::vec3(w, h, d); }

	// Sets the entity's 3D scale using one float value
	// @param - float scale
	void SetScale3D(float scale) { mScale = glm::vec3(scale, scale, scale); }

	// Returns the entity's 2D position
	// @return - glm::vec2 for the position
	glm::vec2 GetPosition2D() const { return glm::vec2(mPosition.x, mPosition.y); }

	// Returns the entity's 2D rotation angle in radians
	// @return - float for rotation in radians
	float GetRotation2D() const { return 2.0f * std::atan2(mRotation.z, mRotation.w); }

	// Gets the entity's float scale
	// @return - float for the scale
	glm::vec2 GetScale2D() const { return glm::vec2(mScale.x, mScale.y); }

	// Gets the entity's rotation as a quaternion
	// @return - const glm::quat& for the rotation
	const glm::quat& GetQuatRotation() const { return mRotation; }

	// Sets the entity's 2D position
	// @param - const glm::vec2& for the position
	void SetPosition2D(const glm::vec2& pos) { mPosition = glm::vec3(pos, 0.0f); }

	// Sets the rotation for a 2D entity
	// @param - const glm::quat& for the rotation
	void SetRotation2D(const glm::quat& rot) { mRotation = glm::normalize(rot); }

	// Sets the entity's float scale
	// @param - float for x scale
	// @param - float for y scale
	void SetScale2D(float x, float y) { mScale.x = x; mScale.y = y; }

	// Gets the forward facing direction of a 3D entity
	// @return - glm::vec3 for the forward
	glm::vec3 GetForward3D() const
	{
		return mRotation * glm::vec3(0.0f, 0.0f, -1.0f);
	}

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

	// Gets the entity's model
	// @return - Model* for the entity's 3D model
	Model* GetModel() { return mModel; }

	// Sets the entity's model
	// @param - Model* for the model
	void SetModel(Model* model) { mModel = model; }

protected:
	// Vector of components the entity uses
	std::vector<Component*> mComponents;

	// Model matrix to transform this entity from model space to world space
	glm::mat4 mModelMatrix;

	// Entity's position
	glm::vec3 mPosition;

	// Entity's rotation
	glm::quat mRotation;

	// Entity's scale
	glm::vec3 mScale;

	Model* mModel;

	// Entity's state
	EntityState mState;
};
