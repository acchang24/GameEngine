#pragma once
#include <glad/glad.h>
#include "VertexLayouts.h"

// VertexBuffer class takes in all the vertex and index information
// of an object and creates an OpenGL Vertex Array Object. This class
// will save the VAO and the vertex/index buffers with an unsigned int
// that can be referenced later with its ID.
class VertexBuffer
{
public:
	// VertexBuffer constructor helps create a Vertex Array Object that can
	// be referenced with mVaoID. A vertex buffer is generated that
	// can be referenced with mVertexBufferID. If the vertex contains
	// indices, then it will create an index buffer as well, which can 
	// be referenced with mIndexBufferID.
	// @param - const void* for the actual vertex data
	// @param - const void* for the actual index data
	// @param - size_t for the size in bytes of the vertex array
	// @param - size_t for the size in bytes of the index array
	// @param - size_t for the number of vertices
	// @param - size_t for the number of indices
	// @param - VertexLayout enum for the layout of the vertex
	VertexBuffer(const void* vertices, const void* indices, size_t vertexSize, size_t indexSize, size_t vertexCount, size_t indexCount, VertexLayout vertexLayout);
	~VertexBuffer();

	// SetVertexAttributePointers sets all the vertex attributes
	// and links the Vertex Attributes with glVertexAttribPointer
	// @param - VertexLayout enum class for the layout of the vertex
	void SetVertexAttributePointers(VertexLayout layout);

	// Sets the VAO as active, then draws based on if it has indices or not
	void Draw();

	// Binds the Vertex Array Object, setting this VAO as the current one.
	// This is set BEFORE every time the vertices are being drawn.
	void SetActive() { glBindVertexArray(mVaoID); }

	unsigned int GetID() const { return mVaoID; }
	unsigned int GetVertexBufferId() const { return mVertexBufferID; }
	unsigned int GetIndexBufferId() const { return mIndexBufferID; }
	size_t GetNumberOfVertices() const { return mVertexCount; }
	size_t GetNumberOfIndices() const { return mIndexCount; }

private:
	// ID for the Vertex Array Object
	unsigned int mVaoID;

	// Reference ID for the vertex buffer
	unsigned int mVertexBufferID;

	// Reference ID for the index buffer (element buffer object)
	unsigned int mIndexBufferID;

	// Number of vertices
	size_t mVertexCount;

	// Number of indices
	size_t mIndexCount;

	// Bool for if the vertex array uses index based drawing
	bool mDrawIndexed;
};