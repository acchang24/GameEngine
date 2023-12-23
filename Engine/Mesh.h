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
	Mesh();
	~Mesh();

	void Draw();

private:
	// The mesh's vertex buffer
	VertexBuffer* mVertexBuffer;

	// The mesh's material
	Material* mMaterial;

	// The mesh's owner
	Entity3D* mOwner;
};