#pragma once

#include <cstdint>
#include <string>

#include "AudioDevice.h"

class __declspec(dllexport) AudioClip
{
public:
	AudioClip();
	~AudioClip();

	static AudioClip* FromFile(const std::string& filePath);

	void* GetBuffer() const { return static_cast<void*>(m_Buffer); };
	const std::string& GetFilePath() const { return m_FilePath; };
	AudioFormat GetFormat() const { return m_Format; };
	uint32_t GetChannelCount() const { return m_Channels; };
	uint64_t GetFrameCount() const { return m_Frames; };
	uint32_t GetSampleRate() const { return m_SampleRate; };
	double GetDuration() const { return m_Duration; };

private:
	ma_audio_buffer* m_Buffer;

	std::string m_FilePath;
	AudioFormat m_Format;
	uint32_t m_Channels;
	uint64_t m_Frames;
	uint32_t m_SampleRate;
	double m_Duration;

	friend class AudioSource;
};