#pragma once
#include "../Entity3D.h"

class VertexBuffer;
class Material;

// 3D cube primitive
class Cube : public Entity3D
{
public:
	Cube();
	~Cube();

	// Set the cube's vertex buffer
	// @param - VertexBuffer* for the new vertex buffer
	void SetVertexBuffer(VertexBuffer* vBuffer) { mVertexBuffer = vBuffer; }
	// Set the cube's material
	// @param - Material* for the new material
	void SetMaterial(Material* m) { mMaterial = m; }

	// Cube specific update
	void OnUpdate(float deltaTime) override;
	// Cube specific draw
	void OnDraw() override;

	void OnDraw(Shader* s) override;
private:
	// Cubes's vertex buffer
	VertexBuffer* mVertexBuffer;

	// Cubes's material
	Material* mMaterial;
};
