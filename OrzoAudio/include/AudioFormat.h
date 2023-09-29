#pragma once

#include <AL/al.h>

enum __declspec(dllexport) AudioFormat : int
{
	None = AL_NONE,
	Mono = AL_FORMAT_MONO16,
	Stereo = AL_FORMAT_STEREO16,
};