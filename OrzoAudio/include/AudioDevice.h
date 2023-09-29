#pragma once
#include <AL/alc.h>

class __declspec(dllexport) AudioDevice
{
public:
	AudioDevice();
	~AudioDevice();

private:
	ALCdevice* p_ALCDevice;
	ALCcontext* p_ALCContext;
};