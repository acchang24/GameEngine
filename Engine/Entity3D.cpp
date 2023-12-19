#include "Entity3D.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "VertexBuffer.h"
#include "Shader.h"
#include "Texture.h"

Entity3D::Entity3D() :
	Entity(),
	mVertexBuffer(nullptr),
	mShader(nullptr),
	mTexture(nullptr),
	mModel(glm::mat4(1.0f)),
	mPosition(glm::vec3(0.0f, 0.0f, 0.0f)),
	mScale(glm::vec3(1.0f, 1.0f, 1.0f)),
	mYaw(0.0f),
	mPitch(0.0f),
	mRoll(0.0f)
{
}

Entity3D::~Entity3D()
{
	std::cout << "Delete entity 3D" << std::endl;
	delete mVertexBuffer;
}

void Entity3D::Update(float deltaTime)
{
	Entity::Update(deltaTime);

	OnUpdate(deltaTime);
}

void Entity3D::Draw()
{
	mShader->SetActive();

	// Send model matrix to shader
	mShader->SetMat4("model", mModel);

	OnDraw();

	if (mTexture)
	{
		mTexture->SetActive();
	}

	mVertexBuffer->Draw();
}

void Entity3D::OnUpdate(float deltaTime)
{
	// Update model matrix (generic)
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
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
