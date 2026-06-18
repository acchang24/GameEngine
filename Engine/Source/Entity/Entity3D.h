#pragma once
#include "Entity.h"
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
	virtual ~Entity3D();

	// Makes this an instanced entity so that multiple instances of the same
	// vertices can be rendered with one draw function call. This generates 
	// a new buffer and loops through all the meshes to enable its array attributes
	// and points them to the data needed per instance.
	// @param - unsigned int for the number of instances to draw
	// @param - const void* for an array of model matrices, and any data needed per instance
	void MakeInstance(unsigned int numInstances, const void* data);

	// Entity3D override of Entity::ProcessInput()
	// @param - const InputSystem* for the input system
	// @param - const EngineContext& for the engine context
	void ProcessInput(const InputSystem* input, const EngineContext& engineContext) override;

	// Override update function for 3D entities
	// @param - float for delta time
	// @param - const EngineContext& for the engine context
	void Update(float deltaTime, const EngineContext& engineContext) override;

	// Override draw function for 3D entities
	void Draw() override;

	// Draw function using a shader set explicitly
	void Draw(Shader* shader);

	// Entity specific ProcessInput code (overridable)
	// @param - const InputSystem* for the input system
	// @param - const EngineContext& for the engine context
	virtual void OnProcessInput(const InputSystem* input, const EngineContext& engineContext);

	// Entity specific update code (overridable)
	// This OnUpdate updates the view matrix
	// @param - float for delta time
	// @param - const EngineContext& for the engine context
	virtual void OnUpdate(float deltaTime, const EngineContext& engineContext);

	// Entity specific draw code (overrideable)
	virtual void OnDraw();

	// OnDraw function using a shader set explicitly
	virtual void OnDraw(Shader* shader);

	void CalculateWorldTransform();

	// Gets the entity's model matrix
	// @return - const glm::mat4& for the entity's model matrix
	const glm::mat4& GetModelMatrix() const { return mModelMatrix; }

	// Gets the entity's model
	// @return - Model* for the entity's 3D model
	Model* GetModel() { return mModel; }
	
	// Set the entity's model matrix
	// @param - const glm::mat4& for the new model matrix
	void SetModelMatrix(const glm::mat4& m) { mModelMatrix = m; }

	// Sets the entity's model
	// @param - Model* for the model
	void SetModel(Model* model) { mModel = model; }

protected:
	// Job to update the Entity3D's model matrix on a separate thread
	class UpdateModelMatrixJob : public JobManager::Job
	{
	public:
		UpdateModelMatrixJob(Entity3D* e) : mEntity(e)
		{}
		// Override Job::DoIt() to update this entity's model matrix
		void DoJob() override;

	private:
		Entity3D* mEntity;
	};

	// Job to update model matrix on seprate thread
	UpdateModelMatrixJob mUpdateModelMatrixJob;

	// Model matrix to transform this entity from model space to world space
	glm::mat4 mModelMatrix;

	// Entity's 3D model
	Model* mModel;

	// Buffer for if this entity is drawn as an instance
	unsigned int mInstanceBuffer;
};
