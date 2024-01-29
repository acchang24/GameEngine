#pragma once
#include "Entity.h"
#include <string>
#include <unordered_map>
#include <assimp/scene.h>
#include "../Graphics/Texture.h"
#include "../Graphics/Material.h"
#include "../Multithreading/JobManager.h"

class Mesh;
class Shader;
class Skeleton;

// Entity3D inherits from the Entity class, and is used for any 3D game
// objects. The class's Update and Draw functions override the parent class.
class Entity3D : public Entity
{
public:
	Entity3D();
	// Entity3D constructor using a model's file.
	// This will load the model using Assimp and create
	// the entity's meshes
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

	// Loads the model's file using Assimp
	// @param - const std::string& for the file name
	bool LoadModel(const std::string& fileName);
	// Recursively goes through the scene's nodes and loads any meshes
	// @param - aiNode*
	// @param - const aiScene*
	// @param - Skeleton* if there is a skeleton for this model
	void ProcessNodes(aiNode* node, const aiScene* scene, Skeleton* skeleton);
	// Takes an assimp mesh and store it in our own Mesh object.
	// It then appends that mesh to the entity's vector of meshes.
	// @param - aiMesh*
	// @param - const aiScene*
	// @param - Skeleton* if there is a skeleton for this model
	Mesh* ProcessMesh(aiMesh* mesh, const aiScene* scene, Skeleton* skeleton);
	// Loads the textures of the material
	// @param - aiMaterial*
	// @param - aiTextureType
	// @param - Material* for the material
	void LoadMaterialTextures(aiMaterial* mat, aiTextureType aiTextureType, Material* material);

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

	// Gets a material by name through the entity's material map
	// @param - const std::string& for the name of the material
	// @return - Material* for the desired material
	Material* GetMaterial(const std::string& name) { return mMaterialMap[name]; }
	// Uses the material map to help sets a particular material's shader. This will
	// affect any mesh using that particular material.
	// @param - const std::string& for the name of the material
	// @param - Shader* for the new shader
	void SetMaterialShader(const std::string& name, Shader* s) { mMaterialMap[name]->SetShader(s); }

	// Adds a mesh to the entity's vector of meshes
	// @param - Mesh* for the new mesh
	void AddMesh(Mesh* m) { mMeshes.emplace_back(m); }
	// Gets the enitity's vector of meshes (can change data)
	// @return - std::vector<Mesh*>& for the vector of meshes
	std::vector<Mesh*>& GetMeshes() { return mMeshes; }
	// Gets the entity's number of meshes
	// @return - size_t for the number of meshes
	size_t GetNumMeshes() const { return mMeshes.size(); }

	// Gets the entity's model matrix
	// @return - const glm::mat4& for the entity's model matrix
	const glm::mat4& GetModelMatrix() const { return mModel; }
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

	// Entity's vector of meshes
	std::vector<Mesh*> mMeshes;

	// Map of the entity's materials. Each mesh of this entity
	// is indexed to one of these materials.
	std::unordered_map<std::string, Material*> mMaterialMap;

	// Job to update model matrix on seprate thread
	UpdateModelMatrixJob mUpdateModelMatrixJob;

	// The entity's model directory
	std::string mDirectory;

	// Entity's model matrix
	glm::mat4 mModel;

	// Entity's position
	glm::vec3 mPosition;

	// Entity's scale
	glm::vec3 mScale;

	// Buffer for if this entity is drawn as an instance
	unsigned int mInstanceBuffer;

	// Yaw rotation
	float mYaw;
	// Pitch rotation
	float mPitch;
	// Roll rotation
	float mRoll;

	int numMesh;
	int numMats;
	int numTextures;
};
