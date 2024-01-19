#include "Sphere.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "VertexLayouts.h"
#include "VertexBuffer.h"
#include "Material.h"
#include "Shader.h"

Sphere::Sphere(float radius, const glm::vec4& color) :
	Entity3D(),
    mVertexBuffer(nullptr),
    mMaterial(nullptr)
{
    int longitudes = 40;
    int latitudes = 30;

    float PI = 3.1415926535f;

    float lengthInv = 1.0f / radius;

    float deltaLatitude = PI / latitudes;
    float deltaLongitude = 2 * PI / longitudes;
    float latitudeAngle;
    float longitudeAngle;

    std::vector<VertexSimple> vertices;
    std::vector<unsigned int> indices;

    for (int i = 0; i <= latitudes; ++i)
    {
        latitudeAngle = PI / 2 - i * deltaLatitude;
        float xy = radius * cosf(latitudeAngle);
        float z = radius * sinf(latitudeAngle);

        VertexSimple v = {};
        for (int j = 0; j <= longitudes; ++j)
        {
            longitudeAngle = j * deltaLongitude;

            float vx = xy * cosf(longitudeAngle);
            float vy = xy * sinf(longitudeAngle);
            float vz = z;

            glm::vec3 pos(vx, vy, vz);
            glm::vec2 uv((float)j / longitudes, (float)i / latitudes);
            glm::vec3 normal(vx* lengthInv, vy* lengthInv, vz* lengthInv);

            v.pos = pos;
            v.normal = normal;
            v.uv = uv;
            vertices.emplace_back(v);
        }
    }
    unsigned int k1, k2;
    for (int i = 0; i < latitudes; ++i)
    {
        k1 = i * (longitudes + 1);
        k2 = k1 + longitudes + 1;
        for (int j = 0; j < longitudes; ++j, ++k1, ++k2)
        {
            if (i != 0)
            {
                indices.emplace_back(k1);
                indices.emplace_back(k2);
                indices.emplace_back(k1 + 1);
            }

            if (i != (latitudes - 1))
            {
                indices.emplace_back(k1 + 1);
                indices.emplace_back(k2);
                indices.emplace_back(k2 + 1);
            }
        }
    }

    size_t vertexSize = sizeof(VertexSimple) * vertices.size();
    size_t indexSize = sizeof(unsigned int) * indices.size();
 
    mVertexBuffer = new VertexBuffer(vertices.data(), indices.data(), vertexSize, indexSize, vertices.size(), indices.size(), VertexLayout::VertexSimple);
}

Sphere::~Sphere()
{
    std::cout << "Delete sphere" << std::endl;
    delete mVertexBuffer;
}

void Sphere::OnUpdate(float deltaTime)
{
	Entity3D::OnUpdate(deltaTime);
}

void Sphere::OnDraw()
{
    mMaterial->SetActive();
    mMaterial->GetShader()->SetMat4("model", mModel);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    mVertexBuffer->Draw();
}
