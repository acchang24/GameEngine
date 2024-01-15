#include "Entity3D.h"
#include <iostream>
#include <queue>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include "Mesh.h"
#include "VertexLayouts.h"
#include "VertexBuffer.h"
#include "AssetManager.h"

Entity3D::Entity3D() :
	Entity(),
	mDirectory(),
	mModel(glm::mat4(1.0f)),
	mPosition(glm::vec3(0.0f, 0.0f, 0.0f)),
	mScale(glm::vec3(1.0f, 1.0f, 1.0f)),
	mInstanceBuffer(0),
	mYaw(0.0f),
	mPitch(0.0f),
	mRoll(0.0f)
{
	numMesh = 0;
	numMats = 0;
	numTextures = 0;
}

Entity3D::Entity3D(const std::string& fileName):
	Entity(),
	mDirectory(),
	mModel(glm::mat4(1.0f)),
	mPosition(glm::vec3(0.0f, 0.0f, 0.0f)),
	mScale(glm::vec3(1.0f, 1.0f, 1.0f)),
	mInstanceBuffer(0),
	mYaw(0.0f),
	mPitch(0.0f),
	mRoll(0.0f)
{
	numMesh = 0;
	numMats = 0;
	numTextures = 0;
	LoadModel(fileName);
	if (numMesh != mMeshes.size())
	{
		std::cout << "different mesh numbers" << std::endl;
	}
	if (numMats != mMaterialMap.size())
	{
		std::cout << "different material numbers" << std::endl;
	}
}

Entity3D::~Entity3D()
{
	std::cout << "Delete entity 3D" << std::endl;

	for (auto m : mMeshes)
	{
		delete m;
	}
	mMeshes.clear();

	mMaterialMap.clear();

	glDeleteBuffers(1, &mInstanceBuffer);
}

void Entity3D::MakeInstance(unsigned int numInstances, const void* data)
{
	glGenBuffers(1, &mInstanceBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mInstanceBuffer);
	glBufferData(GL_ARRAY_BUFFER, numInstances * sizeof(glm::mat4), data, GL_STATIC_DRAW);

	for (auto m : mMeshes)
	{
		m->GetVertexBuffer()->MakeInstance(numInstances);
	}
}

bool Entity3D::LoadModel(const std::string& fileName)
{
	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(fileName, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR ASSIMP parsing the object's file:: " << import.GetErrorString() << std::endl;
		return false;
	}

	mDirectory = fileName.substr(0, fileName.find_last_of('/') + 1);

	ProcessNodes(scene->mRootNode, scene);

	return true;
}

void Entity3D::ProcessNodes(aiNode* node, const aiScene* scene)
{
	//// Process the node's mesh
	//for (unsigned int i = 0; i < node->mNumMeshes; ++i)
	//{
	//	aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
	//	Mesh* newMesh = ProcessMesh(mesh, scene);
	//	newMesh->SetOwner(this);
	//	AddMesh(newMesh);
	//}
	//// Process any children nodes
	//for (unsigned int i = 0; i < node->mNumChildren; ++i)
	//{
	//	ProcessNodes(node->mChildren[i], scene);
	//}

	std::queue<aiNode*> nodeQ;
	nodeQ.push(node);
	aiNode* currNode = nullptr;
	while (!nodeQ.empty())
	{
		currNode = nodeQ.front();
		for (unsigned int i = 0; i < currNode->mNumMeshes; ++i)
		{
			aiMesh* mesh = scene->mMeshes[currNode->mMeshes[i]];
			Mesh* newMesh = ProcessMesh(mesh, scene);
			newMesh->SetOwner(this);
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

Mesh* Entity3D::ProcessMesh(aiMesh* mesh, const aiScene* scene)
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

			// Bitangent
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
	++numMesh;
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		std::string name = material->GetName().C_Str();
		
		// If the material index is not in the entity's material map, create a new material
		if (mMaterialMap.find(name) == mMaterialMap.end())
		{
			std::cout << name << " " << mesh->mMaterialIndex << std::endl;
			++numMats;
			mat = new Material();
			mat->SetShader(AssetManager::Get()->LoadShader("phong"));
			mMaterialMap[name] = mat;
			// Diffuse textures
			LoadMaterialTextures(material, aiTextureType_DIFFUSE, mat);
			// Specular textures
			LoadMaterialTextures(material, aiTextureType_SPECULAR, mat);
			// Emissive textures
			LoadMaterialTextures(material, aiTextureType_EMISSIVE, mat);

			// Normal maps

			// Height maps

			AssetManager::Get()->SaveMaterial(name, mat);
		}
		else
		{
			// use the material from the material map instead
			mat = mMaterialMap[name];
		}
	}

	VertexBuffer* vb = new VertexBuffer(vertices.data(), indices.data(), sizeof(Vertex) * vertices.size(), sizeof(unsigned int) * indices.size(),
		vertices.size(), indices.size(), VertexLayout::Vertex);

	return new Mesh(vb, mat);
}

void Entity3D::LoadMaterialTextures(aiMaterial* mat, aiTextureType aiTextureType, Material* material)
{
	aiString str;
	std::string path = mDirectory;
	for (unsigned int i = 0; i < mat->GetTextureCount(aiTextureType); ++i)
	{
		if (AI_SUCCESS == mat->GetTexture(aiTextureType, i, &str))
		{
			path = mDirectory + (str.C_Str());

			// See if texture is already loaded
			Texture* t = AssetManager::Get()->LoadTexture(path);
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
				}
				material->AddTexture(texture);
				AssetManager::Get()->SaveTexture(path, texture);
				++numTextures;
			}
			else
			{
				// use the cached texture from AssetManager
				material->AddTexture(t);
			}
		}
	}
}


void Entity3D::Update(float deltaTime)
{
	Entity::Update(deltaTime);

	if (mInstanceBuffer == 0)
	{
		OnUpdate(deltaTime);
	}
}

void Entity3D::Draw()
{
	OnDraw();
}

void Entity3D::Draw(Shader* shader)
{
	if (mInstanceBuffer == 0)
	{
		OnDraw(shader);
	}
}

void Entity3D::OnUpdate(float deltaTime)
{
	mModel = glm::mat4(1.0f);

	// Translate
	mModel = glm::translate(mModel, mPosition);

	// Rotate
	mModel = glm::rotate(mModel, glm::radians(mRoll), glm::vec3(0.0f, 0.0f, 1.0f));
	mModel = glm::rotate(mModel, glm::radians(mPitch), glm::vec3(1.0f, 0.0f, 0.0f));
	mModel = glm::rotate(mModel, glm::radians(mYaw), glm::vec3(0.0f, 1.0f, 0.0f));

	// Scale
	mModel = glm::scale(mModel, mScale);
}

void Entity3D::OnDraw()
{
	for (auto m : mMeshes)
	{
		m->Draw();
	}
}

void Entity3D::OnDraw(Shader* shader)
{
	for (auto m : mMeshes)
	{
		m->Draw(shader);
	}
}
