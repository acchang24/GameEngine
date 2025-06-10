#include "AudioSystem.h"
#include <iostream>
#include <SDL2/SDL_mixer.h>

AudioSystem::AudioSystem()
{

}

AudioSystem::~AudioSystem()
{
	std::cout << "Deleted AudioSystem\n";
}

bool AudioSystem::Init()
{
	// Initialize audio
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		std::cout << "Mix_OpenAudio failed: " << Mix_GetError() << "\n";
		return false;
	}
	return true;
}

void AudioSystem::Shutdown()
{
	// Close Audio
	Mix_CloseAudio();
}
