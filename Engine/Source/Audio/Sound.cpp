#include "Sound.h"
#include <algorithm>
#include <iostream>
#include "../Util/LoggerMacros.h"

SFX::SFX(const std::string& fileName):
	mName(fileName),
	mSoundChunk(nullptr),
	mChannel(0),
	mVolume(MIX_MAX_VOLUME)
{
	LOG_DEBUG("Loading SFX file: " + fileName);
	mSoundChunk = Mix_LoadWAV(fileName.c_str());

	if (!mSoundChunk)
	{
		LOG_WARNING("Failed to load sound file: " + fileName);
		std::cout << "Failed to load sound file: " << fileName << "\n";
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

int SFX::Play(int channel, int loops)
{
	if (mSoundChunk)
	{
		mChannel = Mix_PlayChannel(channel, mSoundChunk, loops);
		if (mChannel != -1)
		{
			Mix_Volume(mChannel, mVolume);
		}
	}
	return mChannel;
}

void SFX::Stop() const
{
	Mix_HaltChannel(mChannel);
}

void SFX::Pause() const
{
	Mix_Pause(mChannel);
}

void SFX::Resume() const
{
	Mix_Resume(mChannel);
}

Music::Music(const std::string& fileName) :
	mName(fileName),
	mMusic(nullptr),
	mVolume(MIX_MAX_VOLUME)
{
	LOG_DEBUG("Loading music file: " + fileName);

	mMusic = Mix_LoadMUS(fileName.c_str());

	if (!mMusic)
	{
		LOG_WARNING("Failed to load music file: " + fileName);
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
