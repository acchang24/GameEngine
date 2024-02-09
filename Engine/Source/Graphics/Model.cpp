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
	mSkeleton(nullptr),
	mNumMeshes(0),
	mNumMaterials(0),
	mNumTextures(0)
{
	if (LoadModel(fileName, entity))
	{
		AssetManager::Get()->SaveModel(fileName, this);
	}
}

Model::~Model()
{
	std::cout << "Delete model" << std::endl;

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
	Assimp::Importer import;
	const aiScene * scene = import.ReadFile(fileName, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices | aiProcess_CalcTangentSpace);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR ASSIMP parsing the object's file:: " << import.GetErrorString() << std::endl;
		return false;
	}

	mDirectory = fileName.substr(0, fileName.find_last_of('/') + 1);

	if (scene->HasAnimations())
	{
		entity->SetIsSkinned(true);

		// Create a new animation component for this model
		mSkeleton = new Skeleton();
		AnimationComponent* animComp = new AnimationComponent(entity, mSkeleton);
	}

	ProcessNodes(scene->mRootNode, scene, mSkeleton);

	for (int i = 0; i < scene->mNumAnimations; ++i)
	{
		std::string animName = scene->mAnimations[i]->mName.C_Str();
		Animation* newAnim = new Animation(scene->mAnimations[i], scene->mRootNode, mSkeleton);
		AssetManager::Get()->SaveAnimation(animName, newAnim);
		mSkeleton->SetAnimation(newAnim);
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

	// Check to see if mesh has already been loaded
	Mesh* newMesh = am->LoadMesh(meshName);

	if (!newMesh)
	{
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		vertices.reserve(static_cast<size_t>(mesh->mNumVertices));
		indices.reserve(static_cast<size_t>(mesh->mNumFaces * 3));

		Vertex vertex = {};
		glm::vec3 vector(0.0f);

		// Loop through vertices and add to our vector of vertices
		for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
		{
			// Vertices
			vector.x = mesh->mVertices[i].x;
			vector.y = mesh->mVertices[i].y;
			vector.z = mesh->mVertices[i].z;
			vertex.pos = vector;

			// Normals
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.normal = vector;

			// Textures
			if (mesh->mTextureCoords[0])
			{
				glm::vec2 uv(0.0f);
				uv.x = mesh->mTextureCoords[0][i].x;
				uv.y = mesh->mTextureCoords[0][i].y;
				vertex.uv = uv;

				// Tangent
				vector.x = mesh->mTangents[i].x;
				vector.y = mesh->mTangents[i].y;
				vector.z = mesh->mTangents[i].z;
				vertex.tangent = vector;

				// Bitangent
				vector.x = mesh->mBitangents[i].x;
				vector.y = mesh->mBitangents[i].y;
				vector.z = mesh->mBitangents[i].z;
				vertex.bitangent = vector;
			}
			else
			{
				vertex.uv = glm::vec2(0.0f, 0.0f);
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

		// Load materials
		Material* mat = nullptr;
		++mNumMeshes;
		if (mesh->mMaterialIndex >= 0)
		{
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
			std::string name = material->GetName().C_Str();

			// If the material index is not in the entity's material map, create a new material
			if (mMaterialMap.find(name) == mMaterialMap.end())
			{
				// Check to see if it's in the asset manager map
				mat = am->LoadMaterial(name);

				// Create a new material if it's not in the asset manager
				if (!mat)
				{
					std::cout << name << " " << mesh->mMaterialIndex << std::endl;
					++mNumMaterials;
					mat = new Material();
					mat->SetShader(am->LoadShader("phong"));
					if (skeleton)
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

					// Height maps

					am->SaveMaterial(name, mat);
				}
			}
			else
			{
				// use the material from the material map instead
				mat = mMaterialMap[name];
			}
		}

		// Add bone id and weights to vertex if there is a skeletal animation
		if (skeleton)
		{
			skeleton->ExtractVertexBoneWeights(vertices, mesh);
		}

		VertexBuffer* vb = new VertexBuffer(vertices.data(), indices.data(), sizeof(Vertex) * vertices.size(), sizeof(unsigned int) * indices.size(),
			vertices.size(), indices.size(), VertexLayout::Vertex);

		newMesh = new Mesh(vb, mat);

		am->SaveMesh(meshName, newMesh);
	}

	// Load skeletal bone data
	if (skeleton)
	{
		skeleton->LoadBoneData(mesh);
	}

	return newMesh;
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

			// See if texture is already loaded
			Texture* t = am->LoadTexture(path);
			if (!t)
			{
				// Create the new texture
				Texture* texture = nullptr;

				switch (aiTextureType)
				{
				case aiTextureType_DIFFUSE:
					texture = new Texture(path, TextureType::Diffuse);
					break;
				case aiTextureType_SPECULAR:
					texture = new Texture(path, TextureType::Specular);
					break;
				case aiTextureType_EMISSIVE:
					texture = new Texture(path, TextureType::Emission);
					break;
				case aiTextureType_NORMALS:
					texture = new Texture(path, TextureType::Normal);
					break;
				}
				material->AddTexture(texture);
				am->SaveTexture(path, texture);
				++mNumTextures;
			}
			else
			{
				// use the cached texture from AssetManager
				material->AddTexture(t);
			}
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
