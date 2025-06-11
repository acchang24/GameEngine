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

	// Plays a SFX
	// @param - const std::string& for the SFX file name
	// @param - int for channel: -1 = use first free channel (defaults to -1)
	// @param - int for loops: -1 = infinite loop, 0 = play once, > 0 = loop however many times (defaults to 0)
	// @return - int for the channel playing this SFX
	int PlaySFX(const std::string& sfxName, int channel = -1, int loops = 0);

	// Stops a sfx
	// @param - const std::string& for sfx file name
	void StopSFX(const std::string& sfxName);

	// Pauses a sfx
	// @param - const std::string& for sfx file name
	void PauseSFX(const std::string& sfxName);

	// Resumes playing a sfx
	// @param - const std::string& for sfx file name
	void ResumeSFX(const std::string& sfxName);

	// Plays a music soundtrack
	// @param - const std::string& for music soundtrack file name
	// @param - int for loops: -1 = infinite loop, 0 = play once, > 0 = loop however many times (defaults to -1)
	void PlayMusic(const std::string& musicName, int loops = -1);

	// Stops a music soundtrack
	// @param - const std::string& for music soundtrack file name
	void StopMusic(const std::string& musicName);

	// Pauses a music soundtrack
	// @param - const std::string& for music soundtrack file name
	void PauseMusic(const std::string& musicName);

	// Resumes playing a music soundtrack
	// @param - const std::string& for music soundtrack file name
	void ResumeMusic(const std::string& musicName);

private:
};
