#pragma once
#include "../Entity/Entity3D.h"

class VertexBuffer;
class Material;

// 3D sphere primitive
class Sphere :public Entity3D
{
public:
	Sphere(float radius, const glm::vec4& color);
	~Sphere();

	// Set the sphere's vertex buffer
	// @param - VertexBuffer* for the new vertex buffer
	void SetVertexBuffer(VertexBuffer* vBuffer) { mVertexBuffer = vBuffer; }
	// Set the sphere's material
	// @param - Material* for the new material
	void SetMaterial(Material* m) { mMaterial = m; }

	// Sphere specific update
	void OnUpdate(float deltaTime) override;
	// Sphere specific draw
	void OnDraw() override;
	void OnDraw(Shader* s) override;
private:
	// Sphere's vertex buffer
	VertexBuffer* mVertexBuffer;

	// Sphere's material
	Material* mMaterial;
};