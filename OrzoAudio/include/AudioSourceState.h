#pragma once

#include <AL/al.h>

enum __declspec(dllexport) AudioSourceState : int
{
	Initial = AL_INITIAL,
	Playing = AL_PLAYING,
	Paused = AL_PAUSED,
	Stopped = AL_STOPPED,
};