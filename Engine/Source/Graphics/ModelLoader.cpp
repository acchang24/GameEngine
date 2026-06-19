#include "ModelLoader.h"
#include <iostream>
#include <queue>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include "../Animation/Animation.h"
#include "../Animation/Skeleton.h"
#include "../MemoryManager/AssetManager.h"
#include "../Util/Logger.h"
#include "Material.h"
#include "Mesh.h"
#include "Model.h"
#include "VertexBuffer.h"


Model* ModelLoader::Load(const std::string& fileName, AssetManager* am)
{
	LOG_DEBUG("Loading model: " + fileName);
	std::cout << "Loading model: " << fileName << "\n";

	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(fileName, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices | aiProcess_CalcTangentSpace);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		LOG_ERROR("ASSIMP parsing the object's file:: " + std::string(import.GetErrorString()));
		std::cout << "ERROR ASSIMP parsing the object's file:: " << import.GetErrorString() << "\n";
		return nullptr;
	}

	Model* model = new Model();
	model->SetName(fileName);

	Skeleton* skeleton = nullptr;

	bool hasAnimations = scene->HasAnimations();

	if (hasAnimations)
	{
		model->SetHasAnimations(true);
		skeleton = new Skeleton(scene, fileName);
		model->SetSkeleton(skeleton);

		// Load animations
		for (int i = 0; i < scene->mNumAnimations; ++i)
		{
			std::string animName = fileName + "/" + scene->mAnimations[i]->mName.C_Str();

			// Check to see if animation was already loaded
			Animation* anim = am->LoadAnimation(animName);

			if (!anim)
			{
				// Create a new animation
				anim = new Animation(scene->mAnimations[i], skeleton, animName);

				// Save the animation into AssetManager
				am->SaveAnimation(animName, anim);
			}
		}
	}

	ModelLoader::ProcessNodes(scene->mRootNode, scene, model, skeleton, am, hasAnimations);

	return model;
}

void ModelLoader::ProcessNodes(aiNode* node, const aiScene* scene, Model* targetModel, Skeleton* skeleton, AssetManager* am, bool hasAnims)
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
			Mesh* newMesh = ModelLoader::ProcessMesh(mesh, scene, targetModel, skeleton, am, hasAnims);
			targetModel->AddMesh(newMesh);
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

Mesh* ModelLoader::ProcessMesh(aiMesh* mesh, const aiScene* scene, Model* targetModel, Skeleton* skeleton, AssetManager* am, bool hasAnims)
{
	std::string meshName = mesh->mName.C_Str();

	LOG_DEBUG("Loading mesh: " + meshName);
	std::cout << "Loading mesh: " << meshName << "\n";

	// Check to see if mesh has already been loaded (only for models that might have the exact same mesh with same name)
	Mesh* newMesh = am->LoadMesh(meshName);

	// Create a new mesh
	if (!newMesh)
	{
		std::vector<unsigned int> indices;
		indices.reserve(static_cast<size_t>(mesh->mNumFaces * 3));

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

		bool hasTextures = mesh->HasTextureCoords(0);

		VertexBuffer* vb = nullptr;

		if (!hasAnims)
		{
			std::vector<Vertex> vertices;
			vertices.reserve(static_cast<size_t>(mesh->mNumVertices));

			// Loop through vertices and add to our vector of vertices
			for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
			{
				vertices.emplace_back(GetVertexData(mesh, hasTextures, i));
			}

			vb = new VertexBuffer(vertices.data(), indices.data(), sizeof(Vertex) * vertices.size(), sizeof(unsigned int) * indices.size(),
				vertices.size(), indices.size(), VertexLayout::Vertex);
		}
		else
		{
			std::vector<VertexAnim> vertices;
			vertices.reserve(static_cast<size_t>(mesh->mNumVertices));

			// Loop through vertices and add to our vector of vertices
			for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
			{
				Vertex v = GetVertexData(mesh, hasTextures, i);

				VertexAnim vertex = { v.pos, v.normal, v.uv, v.tangent, v.bitangent };

				vertices.emplace_back(vertex);
			}

			// Add bone id and weights to vertex
			for (int i = 0; i < mesh->mNumBones; ++i)
			{
				// Get bone id from skeleton
				int boneID = skeleton->GetBoneID(mesh->mBones[i]->mName.C_Str());

				aiVertexWeight* boneWeightsArray = mesh->mBones[i]->mWeights;

				int numWeights = mesh->mBones[i]->mNumWeights;

				for (int weightIndex = 0; weightIndex < numWeights; ++weightIndex)
				{
					int vertexID = boneWeightsArray[weightIndex].mVertexId;
					float weight = boneWeightsArray[weightIndex].mWeight;

					VertexAnim& v = vertices[vertexID];
					for (int j = 0; j < MAX_BONE_INFLUENCE; ++j)
					{
						if (v.boneIDs[j] < 0)
						{
							v.boneIDs[j] = boneID;
							v.weights[j] = weight;
							break;
						}
					}
				}
			}

			vb = new VertexBuffer(vertices.data(), indices.data(), sizeof(VertexAnim) * vertices.size(), sizeof(unsigned int) * indices.size(),
				vertices.size(), indices.size(), VertexLayout::VertexAnim);
		}

		// Load material
		Material* mat = ModelLoader::LoadMaterial(scene, mesh, targetModel, am, hasAnims);

		newMesh = new Mesh(vb, mat);

		am->SaveMesh(meshName, newMesh);
	}

	return newMesh;
}

const Vertex ModelLoader::GetVertexData(const aiMesh* mesh, bool hasTextures, unsigned int index)
{
	Vertex vertex = {};

	// Position
	vertex.pos = glm::vec3(mesh->mVertices[index].x, mesh->mVertices[index].y, mesh->mVertices[index].z);

	// Normals
	vertex.normal = glm::vec3(mesh->mNormals[index].x, mesh->mNormals[index].y, mesh->mNormals[index].z);

	// Textures
	if (hasTextures)
	{
		aiVector3D uv = mesh->mTextureCoords[0][index];
		vertex.uv = glm::vec2(uv.x, uv.y);

		// Tangent
		aiVector3D tangent = mesh->mTangents[index];
		vertex.tangent = glm::vec3(tangent.x, tangent.y, tangent.z);

		// Bitangent
		aiVector3D bitangent = mesh->mBitangents[index];
		vertex.bitangent = glm::vec3(bitangent.x, bitangent.y, bitangent.z);
	}

	return vertex;
}

Material* ModelLoader::LoadMaterial(const aiScene* scene, aiMesh* mesh, Model* targetModel, AssetManager* am, bool hasAnims)
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
			LOG_DEBUG("Loading material: " + name + " " + std::to_string(mesh->mMaterialIndex));
			std::cout << "Loading material: " << name << " " << mesh->mMaterialIndex << "\n";

			mat = new Material();
			mat->SetShader(am->LoadShader("phong"));
			if (hasAnims)
			{
				mat->SetShader(am->LoadShader("skinned"));
			}

			targetModel->SaveMaterial(name, mat);
			// Diffuse textures
			ModelLoader::LoadMaterialTextures(material, aiTextureType_DIFFUSE, mat, targetModel, am);
			// Specular textures
			ModelLoader::LoadMaterialTextures(material, aiTextureType_SPECULAR, mat, targetModel, am);
			// Emissive textures
			ModelLoader::LoadMaterialTextures(material, aiTextureType_EMISSIVE, mat, targetModel, am);
			// Normal textures
			ModelLoader::LoadMaterialTextures(material, aiTextureType_NORMALS, mat, targetModel, am);

			am->SaveMaterial(name, mat);
		}

		return mat;
	}

	return nullptr;
}

void ModelLoader::LoadMaterialTextures(aiMaterial* mat, aiTextureType aiTextureType, Material* material, Model* targetModel, AssetManager* am)
{
	const std::string& directory = targetModel->GetName();

	aiString str;
	std::string path = directory.substr(0, directory.find_last_of('/') + 1);

	for (unsigned int i = 0; i < mat->GetTextureCount(aiTextureType); ++i)
	{
		if (AI_SUCCESS == mat->GetTexture(aiTextureType, i, &str))
		{
			path = path + (str.C_Str());

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
			Texture* t = am->LoadTexture(path, type);

			// Add the texture to the material
			material->AddTexture(t);
		}
	}
}