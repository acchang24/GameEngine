#include "Model.h"
#include <iostream>
#include <queue>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include "../Entity/Entity3D.h"
#include "../Graphics/Mesh.h"
#include "../Graphics/VertexBuffer.h"
#include "../MemoryManager/AssetManager.h"
#include "../Animation/Skeleton.h"
#include "../Graphics/UniformBuffer.h"
#include "../Components/AnimationComponent.h"
#include "../Util/LoggerMacros.h"

Model::Model(const std::string& fileName, Entity3D* entity) :
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

	LOG_DEBUG("Loading model: " + fileName);

	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(fileName, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices | aiProcess_CalcTangentSpace);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		LOG_ERROR("ASSIMP parsing the object's file:: " + std::string(import.GetErrorString()));
		std::cout << "ERROR ASSIMP parsing the object's file:: " << import.GetErrorString() << "\n";
		return false;
	}

	mDirectory = fileName;

	mHasAnimations = scene->HasAnimations();

	Skeleton* skeleton = nullptr;
	
	// Create animation component if there are animations
	if (mHasAnimations)
	{
		AnimationComponent* animComp = new AnimationComponent(entity, scene, fileName);
		skeleton = animComp->GetSkeleton();
	}

	ProcessNodes(scene->mRootNode, scene, skeleton);

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

	LOG_DEBUG("Loading mesh: " + meshName);
	std::cout << "Loading mesh: " << meshName << "\n";

	// Check to see if mesh has already been loaded
	Mesh* newMesh = am->LoadMesh(meshName);

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

		if (!mHasAnimations)
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

				VertexAnim vertex = {v.pos, v.normal, v.uv, v.tangent, v.bitangent};

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
		Material* mat = LoadMaterial(scene, mesh, am);

		newMesh = new Mesh(vb, mat);

		am->SaveMesh(meshName, newMesh);
	}

	return newMesh;
}

const Vertex Model::GetVertexData(const aiMesh* mesh, bool hasTextures, unsigned int index)
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

Material* Model::LoadMaterial(const aiScene* scene, aiMesh* mesh, AssetManager* am)
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
	std::string path = mDirectory.substr(0, mDirectory.find_last_of('/') + 1);

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
