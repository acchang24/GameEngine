#include "Sound.h"
#include <algorithm>
#include <iostream>

SFX::SFX(const std::string& fileName):
	mName(fileName),
	mSoundChunk(nullptr),
	mVolume(MIX_MAX_VOLUME)
{
	mSoundChunk = Mix_LoadWAV(fileName.c_str());

	if (!mSoundChunk)
	{
		std::cout << "Failed to load sound file: " << fileName << "\n";
	}
	else
	{
		mVolume = mSoundChunk->volume;
	}
}

SFX::~SFX()
{
	std::cout << "Deleted SFX: " << mName << "\n";

	if (mSoundChunk)
	{
		Mix_FreeChunk(mSoundChunk);
	}
}

void SFX::Play(int loops)
{
	if (mSoundChunk)
	{
		int channel = Mix_PlayChannel(-1, mSoundChunk, loops);
		if (channel != -1)
		{
			Mix_Volume(channel, mVolume);
		}
	}
}

void SFX::Stop()
{
	Mix_HaltChannel(-1);
}

Music::Music(const std::string& fileName) :
	mName(fileName),
	mMusic(nullptr),
	mVolume(MIX_MAX_VOLUME)
{
	mMusic = Mix_LoadMUS(fileName.c_str());

	if (!mMusic)
	{
		std::cout << "Failed to load sound file: " << fileName << "\n";
	}
}

Music::~Music()
{
	std::cout << "Deleted music soundtrack: " << mName << "\n";

	if (mMusic)
	{
		Mix_FreeMusic(mMusic);
	}
}

void Music::Play(int loops)
{
	if (mMusic)
	{
		Mix_VolumeMusic(mVolume);
		Mix_PlayMusic(mMusic, loops);
	}
}

void Music::Pause()
{
	Mix_PauseMusic();
}

void Music::Resume()
{
	Mix_ResumeMusic();
}

void Music::Stop()
{
	Mix_HaltMusic();
}

void Music::SetVolume(int volume)
{
	mVolume = std::clamp(volume, 0, MIX_MAX_VOLUME);
	Mix_VolumeMusic(mVolume);
}
