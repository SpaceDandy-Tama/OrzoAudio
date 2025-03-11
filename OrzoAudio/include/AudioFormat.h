#pragma once

#include <AL/al.h>

enum __declspec(dllexport) AudioFormat : int
{
	OF_None = AL_NONE,
	OF_Mono = AL_FORMAT_MONO16,
	OF_Stereo = AL_FORMAT_STEREO16,
};