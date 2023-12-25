#include "Entity3D.h"
#include <iostream>
#include <fstream>
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
	mYaw(0.0f),
	mPitch(0.0f),
	mRoll(0.0f)
{
}

Entity3D::Entity3D(const std::string& fileName):
	Entity(),
	mDirectory(),
	mModel(glm::mat4(1.0f)),
	mPosition(glm::vec3(0.0f, 0.0f, 0.0f)),
	mScale(glm::vec3(1.0f, 1.0f, 1.0f)),
	mYaw(0.0f),
	mPitch(0.0f),
	mRoll(0.0f)
{
	LoadModel(fileName);
}

Entity3D::~Entity3D()
{
	std::cout << "Delete entity 3D" << std::endl;

	for (auto m : mMeshes)
	{
		delete m;
	}
	mMeshes.clear();
}

bool Entity3D::LoadModel(const std::string& fileName)
{
	Assimp::Importer import;
	const aiScene * scene = import.ReadFile(fileName, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices);

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
	// Process the node's mesh
	for (unsigned int i = 0; i < node->mNumMeshes; ++i)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		Mesh* newMesh = ProcessMesh(mesh, scene);
		newMesh->SetOwner(this);
		AddMesh(newMesh);
	}
	// Process any children nodes
	for (unsigned int i = 0; i < node->mNumChildren; ++i)
	{
		ProcessNodes(node->mChildren[i], scene);
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
	Material* mat = new Material(*AssetManager::Get()->LoadMaterial("reflection"));
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		// Diffuse textures
		LoadMaterialTextures(material, aiTextureType_DIFFUSE, mat);
		// Specular textures
		LoadMaterialTextures(material, aiTextureType_SPECULAR, mat);
		// Normal maps

		// Height maps

	}

	size_t vertexSize = sizeof(Vertex) * vertices.size();
	size_t indexSize = sizeof(unsigned int) * indices.size();

	VertexBuffer* vb = new VertexBuffer(vertices.data(), indices.data(), vertexSize, indexSize, vertices.size(), indices.size(), VertexLayout::Vertex);

	return new Mesh(vb, mat);
}

void Entity3D::LoadMaterialTextures(aiMaterial* mat, aiTextureType aiTextureType, Material* material)
{
	for (unsigned int i = 0; i < mat->GetTextureCount(aiTextureType); ++i)
	{
		aiString str;
		if (AI_SUCCESS == mat->GetTexture(aiTextureType, i, &str))
		{
			std::string path = mDirectory + (str.C_Str());

			// See if texture is already loaded
			Texture* t = AssetManager::Get()->LoadTexture(path);
			if (!t)
			{
				// Create the new texture
				Texture* texture = new Texture(path);
				
				switch (aiTextureType)
				{
				case aiTextureType_SPECULAR:
					texture->SetType(TextureType::Specular);
					break;
				}
				material->AddTexture(texture);
				AssetManager::Get()->SaveTexture(path, texture);
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

	OnUpdate(deltaTime);
}

void Entity3D::Draw()
{
	OnDraw();
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
