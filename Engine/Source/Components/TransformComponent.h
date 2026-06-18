#pragma once
#include "Component.h"
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

// Transform component allows entities to be in world space. Any object that has position, rotation, scale should have this component.
class TransformComponent : public Component
{
public:
	TransformComponent(Entity* owner);
	~TransformComponent();

	// Calculates and returns the model matrix
	// @return - const glm::mat4& for the model matrix
	const glm::mat4& GetModelMatrix() const;

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
	void SetPosition3D(const glm::vec3& pos) { mPosition = pos; mIsDirty = true; }

	// Sets the entity's 3D Rotation using a quaternion
	// @param - const glm::quat& for the rotation
	void SetRotation3D(const glm::quat& rotation) { mRotation = rotation; mIsDirty = true; }

	// Sets the entity's 3D scale using vector 3
	// @param - const glm::vec3& for the scale
	void SetScale3D(const glm::vec3& scale) { mScale = scale; mIsDirty = true; }

	// Sets the entity's 3D scale using 3 float values
	// @param - float for width
	// @param - float for height
	// @param - float for depth
	void SetScale3D(float w, float h, float d) { mScale = glm::vec3(w, h, d); mIsDirty = true; }

	// Returns the entity's 2D position
	// @return - glm::vec2 for the position
	glm::vec2 GetPosition2D() const { return glm::vec2(mPosition.x, mPosition.y); }

	// Returns the entity's 2D rotation in radians
	// @return - float for rotation in radians
	float GetRotation2D() const { return 2.0f * std::atan2(mRotation.z, mRotation.w); }

	// Returns the entity's 2D rotation in degrees
	// @return - float for the rotation in degrees
	float GetRotationDegrees2D() const { return glm::degrees(GetRotation2D()); }

	// Returns the entity's 2D scale
	// @return - glm::vec2 for the scale
	glm::vec2 GetScale2D() const { return glm::vec2(mScale.x, mScale.y); }

	// Sets the entity's 2D position
	// @param - const glm::vec2& for the position
	void SetPosition2D(const glm::vec2& pos) { mPosition = glm::vec3(pos, 0.0f); mIsDirty = true; }

	// Sets the entity's 2D rotation
	// @param - float for the angle in radians
	void SetRotation2D(float radians) { mRotation = glm::angleAxis(radians, glm::vec3(0.0f, 0.0f, 1.0f)); mIsDirty = true; }

	// Sets the entity's 2D scale using vec2
	// @param - const glm::vec2& for the scale
	void SetScale(const glm::vec2& scale) { mScale = glm::vec3(scale, 1.0f); mIsDirty = true; }

	// Sets the entity's 2D scale using separate float values
	// @param - float for horizontal scale
	// @param - float for vertical scale
	void SetScale(float x, float y) { mScale = glm::vec3(x, y, 1.0f); mIsDirty = true; }

	// Gets the forward facing direction of a 3D entity
	// @return - glm::vec3 for the forward
	glm::vec3 GetForward3D() const
	{
		return mRotation * glm::vec3(0.0f, 0.0f, -1.0f);
	}

	// Gets the forward facing direction of a 2D entity
	// @return - glm::vec2 for the forward
	glm::vec2 GetForward2D() const
	{
		float rot = GetRotation2D();
		return glm::vec2(glm::cos(rot), glm::sin(rot));
	}

private:
	// Model matrix to transform an entity from model space to world space
	mutable glm::mat4 mModelMatrix;
	// Entity's position
	glm::vec3 mPosition;
	// Entity's rotation/orientation
	glm::quat mRotation;
	// Entity's scale
	glm::vec3 mScale;
	// bool flag for calculating model matrix
	mutable bool mIsDirty;
};
