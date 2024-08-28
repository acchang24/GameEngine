#pragma once
#include "Entity.h"
#include <string>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include "../Multithreading/JobManager.h"

class Model;
class Shader;

// Entity3D inherits from the Entity class, and is used for any 3D game
// objects. The class's Update and Draw functions override the parent class.
class Entity3D : public Entity
{
public:
	Entity3D();
	// Entity3D constructor using a model's file.
	// This will check to see if the model has been loaded with the asset manager.
	// It will create a new model object if nullptr. If there is a model, it will
	// check to see if this model has animations and flag this entity as skinned. It 
	// will then create a new animation component and copy a skeleton object and add it
	// to the entity's vector of components.
	// @param - const std::string& for the model's file name
	Entity3D(const std::string& fileName);
	~Entity3D();

	// Makes this an instanced entity so that multiple instances of the same
	// vertices can be rendered with one draw function call. This generates 
	// a new buffer and loops through all the meshes to enable its array attributes
	// and points them to the data needed per instance.
	// @param - unsigned int for the number of instances to draw
	// @param - const void* for an array of model matrices, and any data needed per instance
	void MakeInstance(unsigned int numInstances, const void* data);

	// Override update function for 3D entities
	// @param - float for delta time
	void Update(float deltaTime) override;

	// Override draw function for 3D entities
	void Draw() override;

	// Draw function using a shader set explicitly
	void Draw(Shader* shader);

	// Entity specific update code (overridable)
	// This OnUpdate updates the view matrix
	virtual void OnUpdate(float deltaTime);

	// Entity specific draw code (overrideable)
	virtual void OnDraw();

	// OnDraw function using a shader set explicitly
	virtual void OnDraw(Shader* shader);

	void CalculateWorldTransform();

	// Gets the entity's model matrix
	// @return - const glm::mat4& for the entity's model matrix
	const glm::mat4& GetModelMatrix() const { return mModelMatrix; }
	// Gets the entity's position
	// @return - const glm::vec3& for the entity's position
	const glm::vec3& GetPosition() const { return mPosition; }
	// Gets the entity's scale
	// @return - const glm::vec3& for the entity's scale
	const glm::vec3& GetScale() const { return mScale; }
	// Gets the entity's yaw rotation
	// @return - float for the entity's yaw rotation
	float GetYaw() const { return mYaw; }
	// Gets the entity's pitch rotation
	// @return - float for the entity's pitch rotation
	float GetPitch() const { return mPitch; }
	// Gets the entity's roll rotation
	// @return - float for the entity's roll rotation
	float GetRoll() const { return mRoll; }
	
	// Set the entity's model matrix
	// @param - const glm::mat4& for the new model matrix
	void SetModelMatrix(const glm::mat4& m) { mModelMatrix = m; }
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
	void SetYaw(float yaw)
	{
		mYaw = yaw;

		glm::quat newRotation = glm::angleAxis(glm::radians(yaw), glm::vec3(0.0f, 1.0f, 0.0f));

		mRotation = newRotation * mRotation;
	}
	// Set the entity's pitch rotation
	// @param - float for the new pitch rotation
	void SetPitch(float pitch)
	{
		mPitch = pitch;

		glm::quat newRotation = glm::angleAxis(glm::radians(pitch), glm::vec3(1.0f, 0.0f, 0.0f));

		mRotation = newRotation * mRotation;
	}
	// Set the entity's roll rotation
	// @param - float for the new roll rotation
	void SetRoll(float roll)
	{
		mRoll = roll;

		glm::quat newRotation = glm::angleAxis(glm::radians(roll), glm::vec3(0.0f, 0.0f, 1.0f));

		mRotation = newRotation * mRotation;
	}

	void SetRotation(const glm::quat& rotation) { mRotation = rotation; }

	void SetIsSkinned(bool skin) { mIsSkinned = skin; }

	const glm::quat& GetRotation() const { return mRotation; }

	Model* GetModel() { return mModel; }

protected:
	// Job to update the Entity3D's model matrix on a separate thread
	class UpdateModelMatrixJob : public JobManager::Job
	{
	public:
		UpdateModelMatrixJob(Entity3D* e) : mEntity(e)
		{}
		// Override Job::DoIt() to update this entity's model matrix
		void DoIt() override;

	private:
		Entity3D* mEntity;
	};

	// Job to update model matrix on seprate thread
	UpdateModelMatrixJob mUpdateModelMatrixJob;

	// Model matrix to transform this entity from model space to world space
	glm::mat4 mModelMatrix;

	glm::quat mRotation;

	// Entity's position
	glm::vec3 mPosition;

	// Entity's scale
	glm::vec3 mScale;

	// Entity's 3D model
	Model* mModel;

	// Buffer for if this entity is drawn as an instance
	unsigned int mInstanceBuffer;

	// Yaw rotation
	float mYaw;
	// Pitch rotation
	float mPitch;
	// Roll rotation
	float mRoll;

	// Bool to see if this entity has an animation
	bool mIsSkinned;
};
