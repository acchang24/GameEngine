#pragma once
#include "Entity.h"
#include "glm/glm.hpp"

class VertexBuffer;
class Shader;
class Texture;

// Entity3D inherits from the Entity class, and is used for any 3D game
// objects. The class's Update and Draw functions override the parent class.
class Entity3D : public Entity
{
public:
	Entity3D();
	~Entity3D();

	// Override update function for 3D entities
	// @param - float for delta time
	void Update(float deltaTime) override;

	// Override draw function for 3D entities
	void Draw() override;

	// Entity specific update code (overridable)
	virtual void OnUpdate(float deltaTime);
	// Entity specific draw code (overrideable)
	virtual void OnDraw();

	// Set the entity's vertex buffer
	// @param - VertexBuffer* for the new vertex buffer
	void SetVertexBuffer(VertexBuffer* vBuffer) { mVertexBuffer = vBuffer; }
	// Set the entity's shader
	// @param - Shader* for the new shader
	void SetShader(Shader* shader) { mShader = shader; }
	// Set the entity's texture
	// @param - Texture* for the new texture
	void SetTexture(Texture* texture) { mTexture = texture; }
	// Set the entity's model matrix
	// @param - const glm::mat4& for the new model matrix
	void SetModelMatrix(const glm::mat4& m) { mModel = m; }
	// Set the entity's position
	// @param - const glm::vec3& for the new position
	void SetPosition(const glm::vec3& pos) { mPosition = pos; }
	// Set the entity's scale (using vector 3)
	// @param - const glm::vec3& for the new scale
	void SetScale(const glm::vec3& scale) { mScale = scale; }
	// Set the entity's scale (using single float value)
	// @param - float for the new scale
	void SetScale(float scale) { mScale = glm::vec3(scale, scale, scale); }
	// Set the entity's yaw rotation
	// @param - float for the new yaw rotation
	void SetYaw(float yaw) { mYaw = yaw; }
	// Set the entity's pitch rotation
	// @param - float for the new pitch rotation
	void SetPitch(float pitch) { mPitch = pitch; }
	// Set the entity's roll rotation
	// @param - float for the new roll rotation
	void SetRoll(float roll) { mRoll = roll; }

protected:
	// Entity's vertex buffer
	VertexBuffer* mVertexBuffer;

	// Entity's shader
	Shader* mShader;

	// Entity's texture
	Texture* mTexture;

	// Entity's model matrix
	glm::mat4 mModel;

	// Entity's position
	glm::vec3 mPosition;

	// Entity's scale
	glm::vec3 mScale;

	// Yaw rotation
	float mYaw;
	// Pitch rotation
	float mPitch;
	// Roll rotation
	float mRoll;
};