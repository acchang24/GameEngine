#include "UniformBuffer.h"
#include <iostream>
#include "Shader.h"

UniformBuffer::UniformBuffer(size_t bufferSize, BufferBindingPoint bindingPoint, const char* bufferName) :
	mBufferID(0),
	mBufferSize(bufferSize),
	mBindingPoint(static_cast<unsigned int>(bindingPoint)),
	mBufferName(bufferName)
{
	glGenBuffers(1, &mBufferID);
	glBindBuffer(GL_UNIFORM_BUFFER, mBufferID);
	glBufferData(GL_UNIFORM_BUFFER, mBufferSize, NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	// Define the range of the buffer that links to a uniform binding point
	glBindBufferRange(GL_UNIFORM_BUFFER, mBindingPoint, mBufferID, 0, mBufferSize);
}

UniformBuffer::~UniformBuffer()
{
	std::cout << "Delete uniform buffer: " << mBufferName << std::endl;
	glDeleteBuffers(1, &mBufferID);
}

void UniformBuffer::LinkShader(Shader* shader)
{
	unsigned int shaderID = shader->GetID();

	// Get the relevant block index
	unsigned int uniformBlockIndex = glGetUniformBlockIndex(shaderID, mBufferName);

	// Link shader's uniform block to this uniform's binding point
	glUniformBlockBinding(shaderID, uniformBlockIndex, mBindingPoint);
}

void UniformBuffer::UpdateBufferData(const void* data) const
{
	// Bind buffer to update data
	glBindBuffer(GL_UNIFORM_BUFFER, mBufferID);
	// Update the data
	glBufferSubData(GL_UNIFORM_BUFFER, 0, mBufferSize, data);
	// Unbind
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	// Binding to shader binding point
	glBindBufferBase(GL_UNIFORM_BUFFER, static_cast<unsigned int>(mBindingPoint), mBufferID);
}
