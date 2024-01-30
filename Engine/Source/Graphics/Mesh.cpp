#include "Mesh.h"
#include <iostream>
#include "Material.h"
#include "Shader.h"
#include "VertexBuffer.h"

Mesh::Mesh(VertexBuffer* vb, Material* material) :
	mVertexBuffer(vb),
	mMaterial(material)
{

}

Mesh::~Mesh()
{
	std::cout << "Delete mesh" << std::endl;

	delete mVertexBuffer;
}

void Mesh::Draw(const glm::mat4& modelMatrix)
{
	mMaterial->SetActive();

	mMaterial->GetShader()->SetMat4("model", modelMatrix);

	mVertexBuffer->Draw();
}

void Mesh::Draw(Shader* shader, const glm::mat4& modelMatrix)
{
	mMaterial->SetActive();

	shader->SetActive();
	shader->SetMat4("model", modelMatrix);

	mVertexBuffer->Draw();
}
