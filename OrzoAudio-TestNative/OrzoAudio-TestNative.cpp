#include "OrzoAudio.h"

#include <iostream>

int main()
{
	AudioDevice audioDevice = AudioDevice();
	AudioClip* audioClip = AudioClip::FromFile("ethnic offensive3.ogg");
	AudioSource audioSource = AudioSource(audioClip);
	audioSource.Play();

	std::cout << "FilePath: " << audioClip->GetFilePath() << std::endl;
	std::cout << "Format: " << (audioClip->GetFormat() == AudioFormat::OF_Mono ? "Mono" : "Stereo") << std::endl;
	std::cout << "Channel Count: " << audioClip->GetChannelCount() << std::endl;
	std::cout << "Frame Count(Sample Count / Channel Count): " << audioClip->GetFrameCount() << std::endl;
	std::cout << "Sample Rate(Frequency): " << audioClip->GetSampleRate() << std::endl;
	std::cout << "Duration(seconds): " << audioClip->GetDuration() << std::endl;

	while (audioSource.GetCurrentState() == AudioSourceState::Playing)
	{
		std::cout << "\rCurrent Time: " << audioSource.GetCurrentTime() << " - Current Frame: " << audioSource.GetCurrentFrame();
	}

	std::cin.get();

	return 0;
}