#pragma once
#include "Entity.h"
#include <GLM/glm.hpp>

class Renderer2D;

class Entity2D : public Entity
{
public:
	// Entity2D constructor
	Entity2D(Renderer2D* renderer);
	~Entity2D();

	// Override of ProcessInput for entity
	void ProcessInput(const Uint8* keyState) override;

	// Entity2D specific input process (overridable)
	virtual void OnProcessInput(const Uint8* keyState);

	// Override update function for 2D entities
	// @param - float for delta time
	void Update(float deltaTime) override;

	// Entity2D specific update code (overridable)
	virtual void OnUpdate(float deltaTime);

	// Draw function for 2D entities
	//void Draw() override;

	// Gets the entity's position
	// @return - const glm::vec2& for the entity's position
	const glm::vec2& GetPosition() const { return mPosition; }

	// Gets the entity's scale
	// @return - const glm::vec2& for the entity's scale
	const glm::vec2& GetScale() const { return mScale; }

	// Set the entity's position
	// @param - const glm::vec2& for the new position
	void SetPosition(const glm::vec2& pos) { mPosition = pos; }

	// Set the entity's scale (using vector 3)
	// @param - const glm::vec2& for the new scale
	void SetScale(const glm::vec2& scale) { mScale = scale; }

	// Set the entity's scale (using single float value)
	// @param - float for the new scale
	void SetScale(float scale) { mScale.x = scale; mScale.y = scale; }

protected:
	Renderer2D* mRenderer;

	// Entity's position
	glm::vec2 mPosition;

	// Entity's scale
	glm::vec2 mScale;
};
