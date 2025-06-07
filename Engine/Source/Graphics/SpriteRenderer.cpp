#include "SpriteRenderer.h"
#include <algorithm>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../Components/SpriteComponent.h"
#include "../Entity/Entity.h"
#include "Renderer.h"
#include "Shader.h"
#include "Texture.h"
#include "VertexBuffer.h"

SpriteRenderer::SpriteRenderer(Shader* shader, float width, float height) :
	mProjection(glm::ortho(0.0f, width, height, 0.0f, -1.0f, 1.0f)),
	mShader(shader),
	mVertexBuffer(nullptr),
	mWidth(width),
	mHeight(height)
{
	// Vertex attributes for screen quad that fills the entire screen in Normalized Device Coordinates
	VertexScreenQuad quadVertices[] =
	{
		glm::vec2(-0.5f,  0.5f), glm::vec2(0.0f, 1.0f),
		glm::vec2(0.5f, -0.5f), glm::vec2(1.0f, 0.0f),
		glm::vec2(-0.5f, -0.5f), glm::vec2(0.0f, 0.0f),

		glm::vec2(-0.5f,  0.5f), glm::vec2(0.0f, 1.0f),
		glm::vec2(0.5f,  0.5f), glm::vec2(1.0f, 1.0f),
		glm::vec2(0.5f, -0.5f), glm::vec2(1.0f, 0.0f)
	};
	mVertexBuffer = new VertexBuffer(quadVertices, 0, sizeof(quadVertices), 0, sizeof(quadVertices) / sizeof(VertexScreenQuad), 0, VertexLayout::VertexScreenQuad);
}

SpriteRenderer::~SpriteRenderer()
{
	std::cout << "Deleted SpriteRenderer\n";

	delete mVertexBuffer;
}

void SpriteRenderer::Draw()
{
	if (mShader)
	{
		mShader->SetActive();

		for (auto sprite : mSprites)
		{
			if (sprite->IsVisible())
			{
				glm::mat4 model = glm::mat4(1.0f);

				Entity* e = sprite->GetEntity();

				Texture* tex = sprite->GetCurrentSprite();

				glm::vec2 size = e->GetSize();

				// Translate position
				model = glm::translate(model, glm::vec3(e->GetPos2D(), 0.0f));

				// Rotate
				model = glm::rotate(model, glm::radians(e->GetRotation()), glm::vec3(0.0f, 0.0f, 1.0f));

				// Scale
				model = glm::scale(model, glm::vec3(e->GetFloatScale() * size, 1.0f));

				// Send model and projection matrix to shader
				mShader->SetMat4("model", model);

				mShader->SetMat4("projection", mProjection);

				mShader->SetInt("sprite", tex->GetTextureUnit());

				// Bind the texture
				tex->BindTexture();

				// Draw
				mVertexBuffer->Draw();
			}
		}
	}
}

void SpriteRenderer::AddSprite(SpriteComponent* sprite)
{
	mSprites.emplace_back(sprite);
	
	// Sort the vector by draw order (smalll to largest so lower sprites get drawn first and will be further back)
	std::sort(mSprites.begin(), mSprites.end(), [](SpriteComponent* a, SpriteComponent* b) {
		return a->GetDrawOrder() < b->GetDrawOrder();
	});
}

void SpriteRenderer::RemoveSprite(SpriteComponent* sprite)
{
	auto iter = std::find(mSprites.begin(), mSprites.end(), sprite);
	if (iter != mSprites.end())
	{
		mSprites.erase(iter);
	}
}
