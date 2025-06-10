#pragma once
#include <cstdint>
#include <string>
#include <SDL2/SDL_mixer.h>

class SFX
{
public:
	// SFX constructor:
	// @param - const std::string& for the sound file name
	SFX(const std::string& fileName);
	~SFX();

	// Plays the sound
	// @param - int for channel: -1 = first free channel
	// @param - int for loops: -1 = infinite loop, 0 = play once, > 0 = loop however many times
	int Play(int channel, int loops);

	// Stops all channels playing this sound chunk
	void Stop() const;

	// sets the sfx channel
	// @param - int for the new channel
	void SetChannel(int channel) { mChannel = channel; }

	// Sets the volume of the sound
	void SetVolume(uint8_t volume) { if (volume >= 0 && volume <= 128) { mVolume = volume; } }

private:
	// Name of sound file
	std::string mName;

	// Sound chunk
	Mix_Chunk* mSoundChunk;

	// Current channel this chunk is playing on
	int mChannel;

	// Volume of the sound
	uint8_t mVolume;
};

class Music
{
public:
	// Music constructor:
	// @param - const std::string& for the sound file name
	Music(const std::string& fileName);
	~Music();

	// Plays the music sound track
	// @param - int for loops (defaults to -1 or infinite loop)
	void Play(int loops = -1);

	// Pauses the music sound track
	void Pause();

	// Resumes playing the music sound track
	void Resume();

	// Stops playing the music
	void Stop();

	// Gets the music soundtrack's volume
	// @return - int for the volume
	int GetVolume() const { return mVolume; }

	// Sets the volume by clamping between 0 and 128
	// @param - int for the new volume
	void SetVolume(int volume);

private:
	// Name of sound file
	std::string mName;

	// music soundtrack
	Mix_Music* mMusic;

	// music volume
	int mVolume;
};
