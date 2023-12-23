#include "Mesh.h"
#include <iostream>
#include "Material.h"
#include "Shader.h"
#include "VertexBuffer.h"
#include "Entity3D.h"

Mesh::Mesh(VertexBuffer* vb, Material* material) :
	mVertexBuffer(vb),
	mMaterial(material),
	mOwner(nullptr)
{

}

Mesh::~Mesh()
{
	std::cout << "Delete mesh" << std::endl;

	delete mVertexBuffer;

	delete mMaterial;
}

void Mesh::Draw()
{
	mMaterial->SetActive();
	mMaterial->GetShader()->SetMat4("model", mOwner->GetModelMatrix());

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	mVertexBuffer->Draw();
}