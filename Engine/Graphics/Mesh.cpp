#include "Mesh.h"
#include <iostream>
#include "Material.h"
#include "Shader.h"
#include "VertexBuffer.h"
#include "../Entity/Entity3D.h"

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
}

void Mesh::Draw()
{
	mMaterial->SetActive();
	mMaterial->GetShader()->SetMat4("model", mOwner->GetModelMatrix());

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	mVertexBuffer->Draw();
}

void Mesh::Draw(Shader* shader)
{
	mMaterial->SetActive();
	shader->SetActive();
	shader->SetMat4("model", mOwner->GetModelMatrix());

	mVertexBuffer->Draw();
}
