#include "Sphere.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "VertexLayouts.h"
#include "VertexBuffer.h"

Sphere::Sphere(const glm::vec4& color) :
	Entity3D()
{
    // Create sphere vertices

    float pi = 3.1415926535f;

    int stacks = 15;
    int slices = 15;

    std::vector<unsigned int> indices;
    std::vector<VertexColor> vertices;

    for (int i = 0; i <= stacks; ++i)
    {
        float v = static_cast<float>(i) / static_cast<float>(stacks);
        float phi = v * pi;

        float x = 0.0f;
        float y = 0.0f;
        float z = 0.0f;

        for (int j = 0; j <= slices; ++j)
        {
            float u = (float)j / (float)slices;
            float theta = u * pi * 2.0f;

            x = cosf(theta) * sinf(phi);
            y = cosf(phi);
            z = sinf(theta) * sinf(phi);

            VertexColor v = {};
            v.pos = glm::vec3(x, y, z);
            v.color = color;

            vertices.emplace_back(v);
        }
    }

    // Calc The Index Positions
    for (unsigned int i = 0; i < slices * stacks + slices; ++i)
    {
        indices.emplace_back(i);
        indices.emplace_back(i + slices + 1);
        indices.emplace_back(i + slices);

        indices.emplace_back(i + slices + 1);
        indices.emplace_back(i);
        indices.emplace_back(i + 1);
    }

    size_t vertexSize = sizeof(VertexColor) * vertices.size();
    size_t indexSize = sizeof(unsigned int) * indices.size();

    // Create vertex buffer 
    mVertexBuffer = new VertexBuffer(vertices.data(), indices.data(), vertexSize, indexSize, vertices.size(), indices.size(), VertexLayout::VertexColor);
}

Sphere::~Sphere()
{

}

void Sphere::OnUpdate(float deltaTime)
{
	Entity3D::OnUpdate(deltaTime);
}

void Sphere::OnDraw()
{
    Entity3D::OnDraw();
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}
