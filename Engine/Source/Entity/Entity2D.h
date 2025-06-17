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
	// @param - const Uint8* for the keyboard state
	// @param - Keyboard* for the keyboard
	// @param - const Mouse* for the mouse
	void ProcessInput(const Uint8* keyState, Keyboard* keyboard, const Mouse* mouse) override;

	// Entity specific ProcessInput code (overridable)
	// @param - const Uint8* for the keyboard state
	// @param - Keyboard* for the keyboard
	// @param - const Mouse* for the mouse
	virtual void OnProcessInput(const Uint8* keyState, Keyboard* keyboard, const Mouse* mouse);

	// Override update function for 3D entities
	// @param - float for delta time
	void Update(float deltaTime) override;

	// Entity specific update code (overridable)
	// @param - float for delta time
	virtual void OnUpdate(float deltaTime);

	// Gets the entity's forward facing direction based on its rotation
	// @return - const glm::vec2 for the forward facing direction
	const glm::vec2 GetForward() const { return glm::vec2(cos(glm::radians(mRotation)), sin(glm::radians(mRotation))); }

	// Gets the entity's 2d position
	// @return - const glm::vec2& for the position
	const glm::vec2& GetPosition() const { return mPosition; }

	// Gets the entity's size (width and height)
	// @return - const glm::vec2& for width and height vector
	const glm::vec2& GetSize() const { return mSize; }

	// Gets the entity's rotation as a float value in degrees
	// @return - float for the rotation
	float GetRotation() const { return mRotation; }

	// Gets the entity's float scale
	// @return - float for the scale
	float GetScale() const { return mScale; }

	// Sets the entity's 2D position
	// @param - const glm::vec2& for new pos
	void SetPosition(const glm::vec2& pos) { mPosition = pos; }

	// Sets the size
	// @param - const glm::vec2& for new size
	void SetSize(const glm::vec2& size) { mSize = size; }

	// Sets the entity's single float value rotation in degrees
	// @param - float for the new rotation
	void SetRotation(float rotation) { mRotation = rotation; }

	// Sets the entity's float scale
	// @param - float for the new scale
	void SetScale(float scale) { mScale = scale; }

protected:
	// Position for 2D
	glm::vec2 mPosition;

	// Size for width and height
	glm::vec2 mSize;

	// Entity's rotation in degrees as a float
	float mRotation;

	// Entity's scale in float
	float mScale;
};
