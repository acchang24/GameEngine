#pragma once
#include <string>

class AudioSystem
{
public:
	AudioSystem();
	~AudioSystem();

	// Initializes the audio system
	bool Init();

	// Shutsdown audio system
	void Shutdown();

	// Plays an SFX
	// @param - const std::string& for the SFX file name
	// @param - int for channel: -1 = use first free channel (defaults to -1)
	// @param - int for loops: -1 = infinite loop, 0 = play once, > 0 = loop however many times (defaults to 0)
	int PlaySFX(const std::string& sfxName, int channel = -1, int loops = 0);

	// Stops an sfx
	// @param - const std::string& for sfx file name
	void StopSFX(const std::string& sfxName);

	void PauseSFX(int channel);

	void ResumeSFX(int channel);

	// Plays a music soundtrack
	// @param - const std::string& for music soundtrack file name
	void PlayMusic(const std::string& musicName);

	// Stops a music soundtrack
	// @param - const std::string& for music soundtrack file name
	void StopMusic(const std::string& musicName);

private:

};