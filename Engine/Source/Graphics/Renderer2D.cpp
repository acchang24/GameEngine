#include "Renderer2D.h"
#include <algorithm>
#include <iostream>
#include <SDL2/SDL_image.h>
#include "../Components/SpriteComponent.h"
#include "../Entity/Entity2D.h"

// Window width
static int s_WindowWidth;
// Window height
static int s_WindowHeight;
// Bool for whenever the window is resized
static bool s_Resized = false;

Renderer2D::Renderer2D() :
	mWindow(nullptr),
	m2dRenderer(nullptr),
	mWindowTitle(),
	mIsFullScreen(false)
{
}

Renderer2D::~Renderer2D()
{
	std::cout << "Deleted Renderer2D\n";

	// SDL_DestroyTexuture on all the saved textures
	for (const auto& file : mSpriteTextureMap)
	{
		SDL_DestroyTexture(mSpriteTextureMap[file.first]);
	}
}

Renderer2D* Renderer2D::Get()
{
	static Renderer2D s_Renderer2D;

	return &s_Renderer2D;
}

bool Renderer2D::Init(int width, int height, bool fullscreen, SDL_bool mouseCaptured, const char* title)
{
	s_WindowWidth = width;
	s_WindowHeight = height;
	mIsFullScreen = fullscreen;
	mWindowTitle = title;

	// Inititialize SDL for video and audio, and check if successful
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
	{
		std::cout << "Could not initialize SDL: " << SDL_GetError() << "\n";
		return false;
	}

	// Create the window
	if (mIsFullScreen)
	{
		mWindow = SDL_CreateWindow(
			mWindowTitle,
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			0,
			0,
			SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN_DESKTOP
		);
	}
	else
	{
		mWindow = SDL_CreateWindow(
			mWindowTitle,
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			s_WindowWidth,
			s_WindowHeight,
			SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
		);
	}

	// Check if window creation was successful
	if (!mWindow)
	{
		std::cout << "Failed to create a window " << SDL_GetError() << "\n";
		return false;
	}


	// Create the renderer
	m2dRenderer = SDL_CreateRenderer(
		mWindow,
		-1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	);

	// Initizlize SDL Image with IMG_Init
	IMG_Init(IMG_INIT_PNG);

	return true;
}

void Renderer2D::Shutdown()
{
	// IMG_Quit
	IMG_Quit();
	// Destroy the renderer
	SDL_DestroyRenderer(m2dRenderer);
	// Destroy the window
	SDL_DestroyWindow(mWindow);
	// Quit SDL
	SDL_Quit();
}

void Renderer2D::ClearBuffers()
{
	// Set the render draw color to gray
	SDL_SetRenderDrawColor(m2dRenderer, 35, 35, 35, 255);

	// Clear the backbuffer with SDL_RenderClear
	SDL_RenderClear(m2dRenderer);
}

void Renderer2D::Draw()
{
	for (auto s : mSprites)
	{
		if (s->IsVisible())
		{
			s->Draw(m2dRenderer, static_cast<Entity2D*>(s->GetEntity()));
		}
	}
}

void Renderer2D::EndFrame()
{
	SDL_RenderPresent(m2dRenderer);
}

SDL_Texture* Renderer2D::LoadSpriteTexture(const std::string& filename)
{
	SDL_Texture* texture = nullptr;

	if (mSpriteTextureMap.find(filename) != mSpriteTextureMap.end())
	{
		texture = mSpriteTextureMap[filename];
	}
	else
	{
		// Load the texture using IMG_Load
		SDL_Surface* surface = IMG_Load(filename.c_str());
		if (!surface)
		{
			std::cout << "Could not load the sprite texture file << " << filename << "\n";
			return nullptr;
		}
		// Convert surface to SDL_Texture
		texture = SDL_CreateTextureFromSurface(m2dRenderer, surface);

		// Free the SDL_Surface
		SDL_FreeSurface(surface);

		// Add to map
		mSpriteTextureMap[filename] = texture;
	}

	return texture;
}

void Renderer2D::AddSprite(SpriteComponent* sprite)
{
	mSprites.emplace_back(sprite);

	// Sort by draw order
	std::sort(mSprites.begin(), mSprites.end(),
		[](SpriteComponent* a, SpriteComponent* b) {
			return a->GetDrawOrder() < b->GetDrawOrder();
		});
}

void Renderer2D::RemoveSprite(SpriteComponent* sprite)
{
	auto iter = std::find(mSprites.begin(), mSprites.end(), sprite);
	if (iter != mSprites.end())
	{
		// Swap to end of vector and pop off
		auto iter2 = mSprites.end() - 1;
		std::iter_swap(iter, iter2);
		mSprites.pop_back();
	}
}
