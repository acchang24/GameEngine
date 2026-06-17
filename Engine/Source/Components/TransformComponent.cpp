#include "TransformComponent.h"
#include <iostream>

TransformComponent::TransformComponent(Entity* owner) : 
	Component(owner),
	mModelMatrix(glm::mat4(1.0f)),
	mPosition(glm::vec3(0.0f, 0.0f, 0.0f)),
	mRotation(glm::quat()),
	mScale(glm::vec3(1.0f, 1.0f, 1.0f)),
	mIsDirty(true)
{
}

TransformComponent::~TransformComponent()
{
	std::cout << "Deleted Transform Component\n";
}

const glm::mat4& TransformComponent::GetModelMatrix() const
{
	if (mIsDirty)
	{
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, mPosition);
		model = model * glm::mat4_cast(mRotation);
		mModelMatrix = glm::scale(model, mScale);

		mIsDirty = false;
	}

	return mModelMatrix;
}
