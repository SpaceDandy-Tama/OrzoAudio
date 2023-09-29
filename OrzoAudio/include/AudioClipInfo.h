#pragma once

#include "AudioFormat.h"

struct __declspec(dllexport) AudioClipInfo
{
public:
	void* AudioClipPointer = nullptr;
	AudioFormat Format = AudioFormat::None;
	int Channels = 0;
	long long Frames = 0;
	int SampleRate = 0;
	double Duration = 0;
};