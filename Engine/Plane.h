#pragma once
#include "Entity3D.h"

class VertexBuffer;
class Material;
class Shader;

// 3D plane primitive
class Plane : public Entity3D
{
public:
	Plane();
	~Plane();

	// Set the planes's vertex buffer
	// @param - VertexBuffer* for the new vertex buffer
	void SetVertexBuffer(VertexBuffer* vBuffer) { mVertexBuffer = vBuffer; }
	// Set the planes's material
	// @param - Material* for the new material
	void SetMaterial(Material* m) { mMaterial = m; }

	// Plane specific Update
	void OnUpdate(float deltaTime) override;
	// Plane specific Draw
	void OnDraw() override;

	void OnDraw(Shader* s) override;

private:
	// Planes's vertex buffer
	VertexBuffer* mVertexBuffer;

	// Planes's material
	Material* mMaterial;
};