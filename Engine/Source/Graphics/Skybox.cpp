#include "Skybox.h"
#include <iostream>
#include <glm/glm.hpp>
#include "VertexLayouts.h"
#include "VertexBuffer.h"
#include "../MemoryManager/AssetManager.h"
#include "Shader.h"

Skybox::Skybox(const std::vector<std::string>& faceNames) :
    mVertexBuffer(nullptr),
    mShader(nullptr),
    mCubeMap(new CubeMap(faceNames))
{
    // Skybox vertices
    VertexPos vertices[] = {
        glm::vec3(-1.0f,  1.0f, -1.0f),
        glm::vec3(-1.0f, -1.0f, -1.0f),
        glm::vec3(1.0f, -1.0f, -1.0f),
        glm::vec3(1.0f, -1.0f, -1.0f),
        glm::vec3(1.0f,  1.0f, -1.0f),
        glm::vec3(-1.0f,  1.0f, -1.0f),

        glm::vec3(-1.0f, -1.0f,  1.0f),
        glm::vec3(-1.0f, -1.0f, -1.0f),
        glm::vec3(-1.0f,  1.0f, -1.0f),
        glm::vec3(-1.0f,  1.0f, -1.0f),
        glm::vec3(-1.0f,  1.0f,  1.0f),
        glm::vec3(-1.0f, -1.0f,  1.0f),

        glm::vec3(1.0f, -1.0f, -1.0f),
        glm::vec3(1.0f, -1.0f,  1.0f),
        glm::vec3(1.0f,  1.0f,  1.0f),
        glm::vec3(1.0f,  1.0f,  1.0f),
        glm::vec3(1.0f,  1.0f, -1.0f),
        glm::vec3(1.0f, -1.0f, -1.0f),

        glm::vec3(-1.0f, -1.0f,  1.0f),
        glm::vec3(-1.0f,  1.0f,  1.0f),
        glm::vec3(1.0f,  1.0f,  1.0f),
        glm::vec3(1.0f,  1.0f,  1.0f),
        glm::vec3(1.0f, -1.0f,  1.0f),
        glm::vec3(-1.0f, -1.0f,  1.0f),

        glm::vec3(-1.0f,  1.0f, -1.0f),
        glm::vec3(1.0f,  1.0f, -1.0f),
        glm::vec3(1.0f,  1.0f,  1.0f),
        glm::vec3(1.0f,  1.0f,  1.0f),
        glm::vec3(-1.0f,  1.0f,  1.0f),
        glm::vec3(-1.0f,  1.0f, -1.0f),

        glm::vec3(-1.0f, -1.0f, -1.0f),
        glm::vec3(-1.0f, -1.0f,  1.0f),
        glm::vec3(1.0f, -1.0f, -1.0f),
        glm::vec3(1.0f, -1.0f, -1.0f),
        glm::vec3(-1.0f, -1.0f,  1.0f),
        glm::vec3(1.0f, -1.0f,  1.0f)
    };

    mVertexBuffer = new VertexBuffer(vertices, 0, sizeof(vertices), 0, sizeof(vertices) / sizeof(VertexPos), 0, VertexLayout::VertexPos);
}

Skybox::~Skybox()
{
    std::cout << "Delete skybox" << std::endl;

    delete mVertexBuffer;

    delete mCubeMap;
}

void Skybox::Draw(const glm::mat4& view, const glm::mat4& proj)
{
    // Remove the translation from the view matrix (this is so that cube map won't move)
    glm::mat4 viewProj = proj* glm::mat4(glm::mat3(view));

    // change depth function so depth test passes when values are equal to depth buffer's content
    glDepthFunc(GL_LEQUAL);

    mShader->SetActive();

    // Set the right texture unit
    mShader->SetInt("cubeMap", static_cast<int>(TextureType::CubeMap));

    // Set the new view * proj matrix
    mShader->SetMat4("viewProjection", viewProj);

    mCubeMap->BindCubeMap();

    mVertexBuffer->Draw();

    // Set depth function back to default
    glDepthFunc(GL_LESS);
}
