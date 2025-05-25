#pragma once
#include <glm/glm.hpp>

class VertexBuffer;
class Material;
class Shader;

// Mesh class containes all the relvant data required for
// rendering an object. It contains all vertex positions,
// indices, normals, texture voordinates, faces, and material.
class Mesh
{
public:
	// Mesh constructor:
	// @param - VertexBuffer* for the mesh's vertex buffer
	// @param - Material* for the mesh's material
	Mesh(VertexBuffer* vb, Material* material);
	// Mesh destructor:
	// mMaterial is owned/deleted by AssetManager
	// Don't call delete on any materials here.
	~Mesh();

	void Draw(const glm::mat4& modelMatrix);

	// Draw function using a shader set explicitly
	void Draw(Shader* shader, const glm::mat4& modelMatrix);

	VertexBuffer* GetVertexBuffer() { return mVertexBuffer; }

	// Gets the mesh's material
	// @return - Material* for the mesh's material
	Material* GetMaterial() { return mMaterial; }

	// Sets the mesh's material
	// @param - Material* for the new material
	void SetMaterial(Material* material) { mMaterial = material; }

private:
	// The mesh's vertex buffer
	VertexBuffer* mVertexBuffer;

	// The mesh's material
	Material* mMaterial;
};
