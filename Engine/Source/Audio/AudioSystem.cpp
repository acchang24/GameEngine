#include "AudioSystem.h"
#include <iostream>
#include <SDL2/SDL_mixer.h>
#include "../MemoryManager/AssetManager.h"
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

int AudioSystem::PlaySFX(const std::string& sfxName, int channel, int loops)
{
	SFX* sfx = AssetManager::LoadSFX(sfxName);

	if (!sfx)
	{
		return -1;
	}

	return sfx->Play(channel, loops);
}

void AudioSystem::StopSFX(const std::string& sfxName)
{
	SFX* sfx = AssetManager::LoadSFX(sfxName);

	if (sfx)
	{
		sfx->Stop();
	}
}

void AudioSystem::PauseSFX(const std::string& sfxName)
{
	SFX* sfx = AssetManager::LoadSFX(sfxName);

	if (sfx)
	{
		sfx->Pause();
	}
}

void AudioSystem::ResumeSFX(const std::string& sfxName)
{
	SFX* sfx = AssetManager::LoadSFX(sfxName);

	if (sfx)
	{
		sfx->Resume();
	}
}

void AudioSystem::PlayMusic(const std::string& musicName, int loops)
{
	Music* music = AssetManager::LoadMusic(musicName);

	if (music)
	{
		music->Play(loops);
	}
}

void AudioSystem::StopMusic(const std::string& musicName)
{
	Music* music = AssetManager::LoadMusic(musicName);

	if (music)
	{
		music->Stop();
	}
}

void AudioSystem::PauseMusic(const std::string& musicName)
{
	Music* music = AssetManager::LoadMusic(musicName);

	if (music)
	{
		music->Pause();
	}
}

void AudioSystem::ResumeMusic(const std::string& musicName)
{
	Music* music = AssetManager::LoadMusic(musicName);

	if (music)
	{
		music->Resume();
	}
}
