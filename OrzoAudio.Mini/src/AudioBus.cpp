#include "../include/AudioBus.h"

AudioBus::AudioBus(const std::string name) : m_Name{ name }
{
	m_Initialized = ma_sound_group_init(AudioDevice::GetEngine(), 0, nullptr, &m_Group) == MA_SUCCESS;
}

AudioBus::~AudioBus()
{
	if (m_Initialized)
		ma_sound_group_uninit(&m_Group);
}

const std::string& AudioBus::GetName() const
{
	return m_Name;
}

void AudioBus::SetVolume(float volume)
{
	if (m_Initialized)
		ma_sound_group_set_volume(&m_Group, volume);
}

float AudioBus::GetVolume()
{
	return m_Initialized ? ma_sound_group_get_volume(&m_Group) : 0.0f;
}
