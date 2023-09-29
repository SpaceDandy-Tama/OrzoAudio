#pragma once

#include <string>
#include <inttypes.h>

#include <sndfile.h>
#include <AL/al.h>

#include "AudioFormat.h"
#include <iostream>

class __declspec(dllexport) AudioClip
{
public:
	AudioClip();
	~AudioClip();

	static AudioClip* FromFile(const std::string& filePath);

	unsigned int GetBufferID() { return m_BufferID; };

	std::string GetFilePath() { return m_FilePath; };
	AudioFormat GetFormat() { return m_Format; };
	int GetChannelCount() { return m_Channels; };
	long long GetFrameCount() { return m_Frames; };
	int GetSampleRate() { return m_SampleRate; };
	double GetDuration() { return m_Duration; };

private:
	unsigned int m_BufferID;

	std::string m_FilePath;
	AudioFormat m_Format;
	int m_Channels;
	long long m_Frames;
	int m_SampleRate;
	double m_Duration;
};