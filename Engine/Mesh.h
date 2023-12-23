#pragma once

class VertexBuffer;
class Material;
class Entity3D;

// Mesh class containes all the relvant data required for
// rendering an object. It contains all vertex positions,
// indices, normals, texture voordinates, faces, and material.
class Mesh
{
public:
	// Mesh constructor 
	Mesh(VertexBuffer* vb, Material* material);
	~Mesh();

	void Draw();

	// Gets the mesh's material
	// @return - Material* for the mesh's material
	Material* GetMaterial() { return mMaterial; }
	// Gets the mesh's owner
	// @return - Entity3D* for the mesh's owner
	Entity3D* GetOwner() { return mOwner; }

	// Sets the mesh's material
	// @param - Material* for the new material
	void SetMaterial(Material* material) { mMaterial = material; }
	// Sets the mesh's owner
	// @param - Entity3D* for the new owner
	void SetOwner(Entity3D* owner) { mOwner = owner; }

private:
	// The mesh's vertex buffer
	VertexBuffer* mVertexBuffer;

	// The mesh's material
	Material* mMaterial;

	// The mesh's owner
	Entity3D* mOwner;
};