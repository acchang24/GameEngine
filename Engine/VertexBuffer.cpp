#include "VertexBuffer.h"
#include <iostream>

VertexBuffer::VertexBuffer(const void* vertices, const void* indices, size_t vertexSize, size_t indexSize, 
	size_t vertexCount, size_t indexCount, VertexLayout vertexLayout) :
	mVaoID(0),
	mVertexBufferID(0),
	mIndexBufferID(0),
	mVertexCount(0),
	mIndexCount(0),
	mDrawIndexed(false)
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
	// Get the vector of strides based on the vertex layout
	std::vector<int> strides = GetVertexLayoutStrides(layout);

	// Loop through that vector to get the total number of values within each vertex
	int totalStride = 0;
	for (size_t i = 0; i < strides.size(); ++i)
	{
		totalStride += strides[i];
	}

	int spacing = 0;

	// Loop through each attribute of the vertex
	for (size_t i = 0; i < strides.size(); ++i)
	{
		//   Set vertex attributes pointers
		//   Link Vertex Attributes with glVertexAttribPointer():
		// - First argument specifies which vertex attribute to configure. This attribute is specified within the vertex shader
		// - Second argument specifies the size or number of values for the vertex attribute.
		// - Third argument specifies the type of the data, which in this case is a GL_Float (vec* in GLSL)
		// - Fourth argument specifies if the data is going to be normalized.
		// - Fifth argument is the stride, and defines the space between consecutive vertex attributes
		// - Last argument is type void*, and is the offset of where the position data begins in the buffer
		glVertexAttribPointer(i, strides[i], GL_FLOAT, GL_FALSE, totalStride * sizeof(float), (void*)(spacing * sizeof(float)));
		// Enable each attribute
		glEnableVertexAttribArray(i);
		// Increment the spacing by the stride value
		spacing += strides[i];
	}
}

void VertexBuffer::Draw()
{
	SetActive();

	if (mDrawIndexed)
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
}
