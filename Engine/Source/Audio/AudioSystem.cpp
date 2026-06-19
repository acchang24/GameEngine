#include "AudioSystem.h"
#include <iostream>
#include <SDL2/SDL_mixer.h>
#include "Sound.h"

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
	std::cout << "Shutting down AudioSystem\n";

	// Close Audio
	Mix_CloseAudio();
}

int AudioSystem::PlaySFX(SFX* sfx, int channel, int loops)
{
	if (!sfx)
	{
		return -1;
	}

	return sfx->Play(channel, loops);
}

void AudioSystem::StopSFX(SFX* sfx)
{
	if (sfx)
	{
		sfx->Stop();
	}
}

void AudioSystem::PauseSFX(SFX* sfx)
{
	if (sfx)
	{
		sfx->Pause();
	}
}

void AudioSystem::ResumeSFX(SFX* sfx)
{
	if (sfx)
	{
		sfx->Resume();
	}
}

void AudioSystem::PlayMusic(Music* music, int loops)
{
	if (music)
	{
		music->Play(loops);
	}
}

void AudioSystem::StopMusic(Music* music)
{
	if (music)
	{
		music->Stop();
	}
}

void AudioSystem::PauseMusic(Music* music)
{
	if (music)
	{
		music->Pause();
	}
}

void AudioSystem::ResumeMusic(Music* music)
{
	if (music)
	{
		music->Resume();
	}
}
