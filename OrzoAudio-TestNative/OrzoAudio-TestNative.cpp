#include "OrzoAudio.h"
#include <iostream>
#include <atomic>
#include <thread>
#include <chrono>
#include <windows.h>
#undef GetCurrentTime  // remove the macro to use your function

std::atomic<bool> stopRequested(false);

// Console handler for Ctrl+C and close button
BOOL WINAPI ConsoleHandler(DWORD event)
{
	switch (event)
	{
	case CTRL_C_EVENT:
	case CTRL_BREAK_EVENT:
	case CTRL_CLOSE_EVENT:
	case CTRL_LOGOFF_EVENT:
	case CTRL_SHUTDOWN_EVENT:
		stopRequested = true; // just set a flag
		return TRUE;          // tell system we've handled it
	default:
		return FALSE;
	}
}

int main()
{
	// Register console handler
	SetConsoleCtrlHandler(ConsoleHandler, TRUE);

	AudioDevice* audioDevice = new AudioDevice();
	AudioClip* audioClip = AudioClip::FromFile("ethnic offensive3.ogg");
	AudioSource* audioSource = new AudioSource(audioClip);
	audioSource->Play();

	std::cout << "FilePath: " << audioClip->GetFilePath() << std::endl;
	std::cout << "Format: " << (audioClip->GetFormat() == AudioFormat::OF_Mono ? "Mono" : "Stereo") << std::endl;
	std::cout << "Channel Count: " << audioClip->GetChannelCount() << std::endl;
	std::cout << "Frame Count(Sample Count / Channel Count): " << audioClip->GetFrameCount() << std::endl;
	std::cout << "Sample Rate(Frequency): " << audioClip->GetSampleRate() << std::endl;
	std::cout << "Duration(seconds): " << audioClip->GetDuration() << std::endl;

	while (audioSource->GetCurrentState() == AudioSourceState::Playing && !stopRequested)
	{
		std::cout << "\rCurrent Time: " << audioSource->GetCurrentTime() << " - Current Frame: " << audioSource->GetCurrentFrame();
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}

	std::cout << std::endl << "Closing..." << std::endl;

	delete audioSource;
	delete audioClip;
	delete audioDevice;

	return 0;
}