#pragma once

class Music;
class SFX;

class AudioSystem
{
public:
	AudioSystem();
	~AudioSystem();

	// Initializes the audio system
	bool Init();

	// Shutsdown audio system
	void Shutdown();

	// Plays a SFX
	// @param - SFX* for the sound
	// @param - int for channel: -1 = use first free channel (defaults to -1)
	// @param - int for loops: -1 = infinite loop, 0 = play once, > 0 = loop however many times (defaults to 0)
	// @return - int for the channel playing this SFX
	int PlaySFX(SFX* sound, int channel = -1, int loops = 0);

	// Stops a sfx
	// @param - SFX* for the sound
	void StopSFX(SFX* sound);

	// Pauses a sfx
	// @param - SFX* for the sound
	void PauseSFX(SFX* sound);

	// Resumes playing a sfx
	// @param - SFX* for the sound
	void ResumeSFX(SFX* sound);

	// Plays a music soundtrack
	// @param - Music* for the music soundtrack
	// @param - int for loops: -1 = infinite loop, 0 = play once, > 0 = loop however many times (defaults to -1)
	void PlayMusic(Music* music, int loops = -1);

	// Stops a music soundtrack
	// @param - Music* for the music soundtrack
	void StopMusic(Music* music);

	// Pauses a music soundtrack
	// @param - Music* for the music soundtrack
	void PauseMusic(Music* music);

	// Resumes playing a music soundtrack
	// @param - Music* for the music soundtrack
	void ResumeMusic(Music* music);

private:
};
