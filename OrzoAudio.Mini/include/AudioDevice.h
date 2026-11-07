#pragma once

#include "miniaudio/miniaudio.h"

#include "AudioFormat.h"
#include "Vector.h"

class __declspec(dllexport) AudioDevice
{
public:
	static bool Initialize();
	static void Destroy();
	static bool IsInitialized();

	static AudioFormat GetMixingFormat();
	static AudioFormat GetOutputFormat();
	static unsigned int GetOutputChannels();
	static unsigned int GetOutputSampleRate();

	static void SetListenerPosition(Vector3& pos);
	static Vector3 GetListenerPosition();
	static void SetListenerDirection(Vector3& dir);
	static Vector3 GetListenerDirection();
	static void SetListenerWorldUp(Vector3& dir);
	static Vector3 GetListenerWorldUp();

private:
	static ma_engine m_Engine;
	static ma_engine* GetEngine();
	static ma_format FromAudioFormat(AudioFormat format);
	static AudioFormat ToAudioFormat(ma_format format);

	static bool m_Initialized;

	static Vector3 m_ListenerPos;
	static Vector3 m_ListenerDir;
	static Vector3 m_ListenerWUp;

	friend class AudioBus;
	friend class AudioClip;
	friend class AudioSource;
};