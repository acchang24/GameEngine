#include "Model.h"
#include <iostream>
#include <queue>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include "../Entity/Entity3D.h"
#include "../Graphics/Mesh.h"
#include "../Graphics/VertexLayouts.h"
#include "../Graphics/VertexBuffer.h"
#include "../MemoryManager/AssetManager.h"
#include "../Animation/Skeleton.h"
#include "../Animation/Animation.h"
#include "../Graphics/UniformBuffer.h"
#include "../Profiler/Profiler.h"
#include "../Components/AnimationComponent.h"

Model::Model(const std::string& fileName, Entity3D* entity) :
	mNumMeshes(0),
	mNumMaterials(0),
	mNumTextures(0),
	mHasAnimations(false)
{
	if (LoadModel(fileName, entity))
	{
		AssetManager::Get()->SaveModel(fileName, this);
	}
}

Model::~Model()
{
	std::cout << "Deleted model: " << mDirectory << "\n";

	mMeshes.clear();

	mMaterialMap.clear();
}

void Model::MakeInstance(unsigned int numInstances)
{
	for (auto m : mMeshes)
	{
		m->GetVertexBuffer()->MakeInstance(numInstances);
	}
}

bool Model::LoadModel(const std::string& fileName, Entity3D* entity)
{
	std::cout << "Loading model: " << fileName << "\n";

	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(fileName, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices | aiProcess_CalcTangentSpace);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR ASSIMP parsing the object's file:: " << import.GetErrorString() << "\n";
		return false;
	}

	mDirectory = fileName.substr(0, fileName.find_last_of('/') + 1);

	mHasAnimations = scene->HasAnimations();

	Skeleton* newSkeleton = nullptr;
	if (mHasAnimations)
	{
		entity->SetIsSkinned(true);

		std::cout << "Loading animation for: " << fileName << "\n";

		// Create a new animation component for this model
		newSkeleton = new Skeleton();
		AssetManager::Get()->SaveSkeleton(fileName, newSkeleton);
	}

	ProcessNodes(scene->mRootNode, scene, newSkeleton);

	for (int i = 0; i < scene->mNumAnimations; ++i)
	{
		std::string animName = scene->mAnimations[i]->mName.C_Str();
		Animation* newAnim = new Animation(scene->mAnimations[i], scene->mRootNode, newSkeleton);
		AssetManager::Get()->SaveAnimation(animName, newAnim);
		newSkeleton->SetAnimation(newAnim);
	}

	if (mHasAnimations)
	{
		Skeleton* skeleton = new Skeleton(*newSkeleton);
		AnimationComponent* animComp = new AnimationComponent(entity, skeleton);
	}

	return true;
}

void Model::ProcessNodes(aiNode* node, const aiScene* scene, Skeleton* skeleton)
{
	std::queue<aiNode*> nodeQ;
	nodeQ.push(node);
	aiNode* currNode = nullptr;
	while (!nodeQ.empty())
	{
		currNode = nodeQ.front();
		for (unsigned int i = 0; i < currNode->mNumMeshes; ++i)
		{
			aiMesh* mesh = scene->mMeshes[currNode->mMeshes[i]];
			Mesh* newMesh = ProcessMesh(mesh, scene, skeleton);
			AddMesh(newMesh);
		}
		nodeQ.pop();
		for (unsigned int i = 0; i < currNode->mNumChildren; ++i)
		{
			if (currNode->mChildren[i])
			{
				nodeQ.push(currNode->mChildren[i]);
			}
		}
	}
}

Mesh* Model::ProcessMesh(aiMesh* mesh, const aiScene* scene, Skeleton* skeleton)
{
	AssetManager* am = AssetManager::Get();

	std::string meshName = mesh->mName.C_Str();

	std::cout << "Loading mesh: " << meshName << "\n";

	// Load skeletal bone data if it exists
	if (mHasAnimations)
	{
		skeleton->LoadBoneData(mesh);
	}

	// Check to see if mesh has already been loaded
	Mesh* newMesh = am->LoadMesh(meshName);

	if (!newMesh)
	{
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		vertices.reserve(static_cast<size_t>(mesh->mNumVertices));
		indices.reserve(static_cast<size_t>(mesh->mNumFaces * 3));

		bool hasTextures = mesh->HasTextureCoords(0);

		// Loop through vertices and add to our vector of vertices
		for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
		{
			Vertex vertex = {};

			// Position
			vertex.pos = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);

			// Normals
			vertex.normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);

			// Textures
			if (hasTextures)
			{
				aiVector3D uv = mesh->mTextureCoords[0][i];
				vertex.uv = glm::vec2(uv.x, uv.y);

				// Tangent
				aiVector3D tangent = mesh->mTangents[i];
				vertex.tangent = glm::vec3(tangent.x, tangent.y, tangent.z);

				// Bitangent
				aiVector3D bitangent = mesh->mBitangents[i];
				vertex.bitangent = glm::vec3(bitangent.x, bitangent.y, bitangent.z);
			}

			vertices.emplace_back(vertex);
		}

		// Loop through the mesh's faces to get indices
		for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
		{
			aiFace face = mesh->mFaces[i];
			// Retrieve index info of the face
			for (unsigned int j = 0; j < face.mNumIndices; ++j)
			{
				indices.emplace_back(face.mIndices[j]);
			}
		}

		// Load material
		Material* mat = LoadMaterial(scene, mesh, skeleton, am);

		// Add bone id and weights to vertex if there is a skeletal animation
		if (mHasAnimations)
		{
			skeleton->ExtractVertexBoneWeights(vertices, mesh);
		}

		VertexBuffer* vb = new VertexBuffer(vertices.data(), indices.data(), sizeof(Vertex) * vertices.size(), sizeof(unsigned int) * indices.size(),
			vertices.size(), indices.size(), VertexLayout::Vertex);

		newMesh = new Mesh(vb, mat);

		++mNumMeshes;

		am->SaveMesh(meshName, newMesh);
	}

	return newMesh;
}

Material* Model::LoadMaterial(const aiScene* scene, aiMesh* mesh, Skeleton* skeleton, AssetManager* am)
{
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		std::string name = material->GetName().C_Str();

		// Check to see if it's in the asset manager map
		Material* mat = am->LoadMaterial(name);

		// Create a new material if it's not in the asset manager
		if (!mat)
		{
			std::cout << "Loading material: " << name << " " << mesh->mMaterialIndex << "\n";
			++mNumMaterials;
			mat = new Material();
			mat->SetShader(am->LoadShader("phong"));
			if (mHasAnimations)
			{
				mat->SetShader(am->LoadShader("skinned"));
			}

			mMaterialMap[name] = mat;
			// Diffuse textures
			LoadMaterialTextures(material, aiTextureType_DIFFUSE, mat, am);
			// Specular textures
			LoadMaterialTextures(material, aiTextureType_SPECULAR, mat, am);
			// Emissive textures
			LoadMaterialTextures(material, aiTextureType_EMISSIVE, mat, am);
			// Normal textures
			LoadMaterialTextures(material, aiTextureType_NORMALS, mat, am);

			am->SaveMaterial(name, mat);
		}

		return mat;
	}

	return nullptr;
}

void Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType aiTextureType, Material* material, AssetManager* am)
{
	aiString str;
	std::string path = mDirectory;

	for (unsigned int i = 0; i < mat->GetTextureCount(aiTextureType); ++i)
	{
		if (AI_SUCCESS == mat->GetTexture(aiTextureType, i, &str))
		{
			path = mDirectory + (str.C_Str());

			std::cout << "Loading texture: " << path << "\n";
			
			// Set the texture's type
			TextureType type = TextureType::None;
			switch (aiTextureType)
			{
			case aiTextureType_DIFFUSE:
				type = TextureType::Diffuse;
				break;
			case aiTextureType_SPECULAR:
				type = TextureType::Specular;
				break;
			case aiTextureType_EMISSIVE:
				type = TextureType::Emission;
				break;
			case aiTextureType_NORMALS:
				type = TextureType::Normal;
				break;
			}
			// Load the texture
			Texture* t = AssetManager::LoadTexture(path, type);

			++mNumTextures;
			
			// Add the texture to the material
			material->AddTexture(t);
		}
	}
}

void Model::Draw(const glm::mat4& model)
{
	for (auto m : mMeshes)
	{
		m->Draw(model);
	}
}

void Model::Draw(Shader* s, const glm::mat4& model)
{
	for (auto m : mMeshes)
	{
		m->Draw(s, model);
	}
}
