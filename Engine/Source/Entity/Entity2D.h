#pragma once
#include "Entity.h"
#include <GLM/glm.hpp>

class Entity2D : public Entity
{
public:
	Entity2D();

	// Entity2D constructor:
	// @param - float for width of entity
	// @param - float for height of entity
	Entity2D(float width, float height);
	virtual ~Entity2D();

	// Entity2D override of Entity::ProcessInput()
	// @param - const InputSystem* for the input system
	// @param - const EngineContext& for the engine context
	void ProcessInput(const InputSystem* input, const EngineContext& engineContext) override;

	// Entity specific ProcessInput code (overridable)
	// @param - const InputSystem* for the input system
	// @param - const EngineContext& for the engine context
	virtual void OnProcessInput(const InputSystem* input, const EngineContext& engineContext);

	// Override update function for 3D entities
	// @param - float for delta time
	// @param - const EngineContext& for the engine context
	void Update(float deltaTime, const EngineContext& engineContext) override;

	// Entity specific update code (overridable)
	// @param - float for delta time
	// @param - const EngineContext& for the engine context
	virtual void OnUpdate(float deltaTime, const EngineContext& engineContext);

	// Gets the entity's forward facing direction based on its rotation
	// @return - const glm::vec2 for the forward facing direction
	const glm::vec2 GetForward() const { return glm::vec2(cos(glm::radians(mRotation)), sin(glm::radians(mRotation))); }

	// Gets the entity's 2d position
	// @return - const glm::vec2& for the position
	const glm::vec2& GetPosition() const { return mPosition; }

	// Gets the entity's rotation as a float value in degrees
	// @return - float for the rotation
	float GetRotation() const { return mRotation; }

	// Gets the entity's float scale
	// @return - float for the scale
	float GetScale() const { return mScale; }

	// Sets the entity's 2D position
	// @param - const glm::vec2& for new pos
	void SetPosition(const glm::vec2& pos) { mPosition = pos; }

	// Sets the entity's single float value rotation in degrees
	// @param - float for the new rotation
	void SetRotation(float rotation) { mRotation = rotation; }

	// Sets the entity's float scale
	// @param - float for the new scale
	void SetScale(float scale) { mScale = scale; }

protected:
	// Position for 2D
	glm::vec2 mPosition;

	// Entity's rotation in degrees as a float
	float mRotation;

	// Entity's scale in float
	float mScale;
};
