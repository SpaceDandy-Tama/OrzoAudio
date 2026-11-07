#pragma once

#include <string>

#include "AudioDevice.h"

class __declspec(dllexport) AudioBus
{
public:
	AudioBus(const std::string name);
	~AudioBus();

	const std::string& GetName() const;
	void SetVolume(float volume);
	float GetVolume();

private:
	ma_sound_group m_Group{};

	std::string m_Name;
	bool m_Initialized = false;

	friend class AudioSource;
};