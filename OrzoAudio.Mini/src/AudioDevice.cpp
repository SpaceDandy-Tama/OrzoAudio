#include <iostream>

#include "../include/AudioDevice.h"

bool AudioDevice::m_Initialized = false;
ma_engine AudioDevice::m_Engine{};

Vector3 AudioDevice::m_ListenerPos = Vector3{ 0, 0, 0 };
Vector3 AudioDevice::m_ListenerDir = Vector3{ 0, 0, 1 };
Vector3 AudioDevice::m_ListenerWUp = Vector3{ 0, 1, 0 };

bool AudioDevice::Initialize()
{
	if (AudioDevice::m_Initialized)
		return false;

	if (ma_engine_init(nullptr, &AudioDevice::m_Engine) != MA_SUCCESS)
	{
		std::cout << "Failed to Initialize the AudioDevice.";
		return false;
	}

	SetListenerPosition(m_ListenerPos);
	SetListenerDirection(m_ListenerDir);
	SetListenerWorldUp(m_ListenerWUp);

#if _DEBUG
	ma_device* device = ma_engine_get_device(&AudioDevice::m_Engine);
	std::cout << device->sampleRate << " Hz output | internal: " << device->playback.internalSampleRate << std::endl;
	std::cout << device->playback.channels << " channels output | internal: " << device->playback.internalChannels << std::endl;
	std::cout << device->playback.format << " format output | internal: " << device->playback.internalFormat << std::endl;
	std::cout << std::endl;
#endif

	return AudioDevice::m_Initialized = true;
}

void AudioDevice::Destroy()
{
	if (!m_Initialized)
		return;

	ma_engine_uninit(&AudioDevice::m_Engine);

	AudioDevice::m_Initialized = false;
}

bool AudioDevice::IsInitialized()
{
	return AudioDevice::m_Initialized;
}

AudioFormat AudioDevice::GetMixingFormat()
{
	return AudioDevice::m_Initialized ? AudioDevice::ToAudioFormat(ma_engine_get_device(&AudioDevice::m_Engine)->playback.internalFormat) : AudioFormat::OF_None;
}
AudioFormat AudioDevice::GetOutputFormat()
{
	return AudioDevice::m_Initialized ? AudioDevice::ToAudioFormat(ma_engine_get_device(&AudioDevice::m_Engine)->playback.format) : AudioFormat::OF_None;
}
unsigned int AudioDevice::GetOutputChannels()
{
	return AudioDevice::m_Initialized ? ma_engine_get_device(&AudioDevice::m_Engine)->playback.channels : 0;
}
unsigned int AudioDevice::GetOutputSampleRate()
{
	return AudioDevice::m_Initialized ? ma_engine_get_device(&AudioDevice::m_Engine)->sampleRate : 0;
}

void AudioDevice::SetListenerPosition(Vector3& pos)
{
	if (!m_Initialized)
		return;

	m_ListenerPos = pos;
	ma_engine_listener_set_position(&AudioDevice::m_Engine, 0, AudioDevice::m_ListenerPos.x, AudioDevice::m_ListenerPos.y, -AudioDevice::m_ListenerPos.z);
}
Vector3 AudioDevice::GetListenerPosition()
{
	return m_ListenerPos;
}
void AudioDevice::SetListenerDirection(Vector3& dir)
{
	if (!m_Initialized)
		return;

	m_ListenerDir = dir;
	ma_engine_listener_set_direction(&AudioDevice::m_Engine, 0, AudioDevice::m_ListenerDir.x, AudioDevice::m_ListenerDir.y, -AudioDevice::m_ListenerDir.z);
}
Vector3 AudioDevice::GetListenerDirection()
{
	return m_ListenerDir;
}
void AudioDevice::SetListenerWorldUp(Vector3& dir)
{
	if (!m_Initialized)
		return;

	m_ListenerWUp = dir;
	ma_engine_listener_set_world_up(&AudioDevice::m_Engine, 0, AudioDevice::m_ListenerWUp.x, AudioDevice::m_ListenerWUp.y, -AudioDevice::m_ListenerWUp.z);
}
Vector3 AudioDevice::GetListenerWorldUp()
{
	return m_ListenerWUp;
}

ma_engine* AudioDevice::GetEngine()
{
	return AudioDevice::m_Initialized ? &AudioDevice::m_Engine : nullptr;
}
ma_format AudioDevice::FromAudioFormat(AudioFormat format)
{
	return static_cast<ma_format>(format);
}
AudioFormat AudioDevice::ToAudioFormat(ma_format format)
{
	return static_cast<AudioFormat>(format);
}