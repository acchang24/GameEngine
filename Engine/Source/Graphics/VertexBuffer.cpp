#include "VertexBuffer.h"
#include <iostream>

VertexBuffer::VertexBuffer(const void* vertices, const void* indices, size_t vertexSize, size_t indexSize,
	size_t vertexCount, size_t indexCount, VertexLayout vertexLayout) :
	mVaoID(0),
	mVertexBufferID(0),
	mIndexBufferID(0),
	mNumInstances(0),
	mLastAttribIndex(0),
	mVertexCount(vertexCount),
	mIndexCount(indexCount),
	mDrawIndexed(false),
	mDrawInstanced(false)
{
	// Create a vertex array object, store it in mVaoID for reference
	glGenVertexArrays(1, &mVaoID);
	// Bind the new vertex array object
	glBindVertexArray(mVaoID);

	// Generate a vertex buffer
	glGenBuffers(1, &mVertexBufferID);
	// Bind the vertex buffer (GL_ARRAY_BUFFER)
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferID);

	// Copy vertex data into the currently bound buffer
	// Pass in buffer target, size of data, and the actual data
	// GL_STREAM_DRAW: the data is set only once and used by the GPU at most a few times.
	// GL_STATIC_DRAW: the data is set only once and used many times.
	// GL_DYNAMIC_DRAW: the data is changed a lot and used many times.
	glBufferData(GL_ARRAY_BUFFER, vertexSize, vertices, GL_STATIC_DRAW);

	// Set mDrawIndexed to true and generate an index buffer if there is an array of indices
	if (indices)
	{
		mDrawIndexed = true;

		// Generate a index buffer
		glGenBuffers(1, &mIndexBufferID);
		// Bind the index buffer (GL_ELEMENT_ARRAY_BUFFER)
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBufferID);
		// Copy index data into currently bound buffer
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexSize, indices, GL_STATIC_DRAW);
	}

	SetVertexAttributePointers(vertexLayout);
}

VertexBuffer::~VertexBuffer()
{
	std::cout << "Delete vertex arrays and vertex/index buffers" << std::endl;
	// De-allocate all resources
	glDeleteVertexArrays(1, &mVaoID);
	glDeleteBuffers(1, &mVertexBufferID);
	glDeleteBuffers(1, &mIndexBufferID);

	mVaoID = 0;
	mVertexBufferID = 0;
	mIndexBufferID = 0;
	mVertexCount = 0;
	mIndexCount = 0;
}

void VertexBuffer::SetVertexAttributePointers(VertexLayout layout)
{
	switch (layout)
	{
	case VertexLayout::VertexPos:
		mLastAttribIndex = VertexPosAttribPointer();
		break;
	case VertexLayout::VertexColor:
		mLastAttribIndex = VertexColorAttribPointer();
		break;
	case VertexLayout::Vertex:
		mLastAttribIndex = VertexAttribPointer();
		break;
	case VertexLayout::VertexSimple:
		mLastAttribIndex = VertexSimpleAttribPointer();
		break;
	case VertexLayout::VertexScreenQuad:
		mLastAttribIndex = VertexScreenQuadAttribPointer();
		break;
	}
}

void VertexBuffer::MakeInstance(unsigned int numInstances)
{
	SetActive();

	mNumInstances = numInstances;
	mDrawInstanced = true;

	glEnableVertexAttribArray(mLastAttribIndex);
	glVertexAttribPointer(mLastAttribIndex, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
	glVertexAttribDivisor(mLastAttribIndex, 1);
	++mLastAttribIndex;
	glEnableVertexAttribArray(mLastAttribIndex);
	glVertexAttribPointer(mLastAttribIndex, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
	glVertexAttribDivisor(mLastAttribIndex, 1);
	++mLastAttribIndex;
	glEnableVertexAttribArray(mLastAttribIndex);
	glVertexAttribPointer(mLastAttribIndex, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
	glVertexAttribDivisor(mLastAttribIndex, 1);
	++mLastAttribIndex;
	glEnableVertexAttribArray(mLastAttribIndex);
	glVertexAttribPointer(mLastAttribIndex, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));
	glVertexAttribDivisor(mLastAttribIndex, 1);
	++mLastAttribIndex;

	glBindVertexArray(0);
}

void VertexBuffer::Draw()
{
	SetActive();

	if (mDrawInstanced)
	{
		glDrawElementsInstanced(GL_TRIANGLES, mIndexCount, GL_UNSIGNED_INT, 0, mNumInstances);
	}
	else if (mDrawIndexed)
	{
		// - First argument specifies the mode to draw in, in this case draw triangles
		// - Second argument is the count or number of elements/indices to draw
		// - Third argument is type of indices which is of GL_UNSIGNED_INT
		// - Last argument allows us to specify offset in EBO or pass in an index array
		glDrawElements(GL_TRIANGLES, mIndexCount, GL_UNSIGNED_INT, 0);
	}
	else
	{
		// - First argument takes the OpenGL primitive type to draw. In this case, draw triangles
		// - Second argument specifies the starting index of the vertex array to draw
		// - Last argument specifies how many vertices to draw
		glDrawArrays(GL_TRIANGLES, 0, mVertexCount);
	}

	glBindVertexArray(0);
}
