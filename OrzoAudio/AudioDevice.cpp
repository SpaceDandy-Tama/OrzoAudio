#include "AudioDevice.h"

#include <AL/al.h>
#include <iostream>

AudioDevice::AudioDevice()
{
	p_ALCDevice = alcOpenDevice(nullptr); //nullptr = get default device
	if (!p_ALCDevice)
		std::cout << "Failed to get sound device" << std::endl;

	p_ALCContext = alcCreateContext(p_ALCDevice, nullptr); // create context
	if (!p_ALCContext)
		std::cout << "Failed to set sound context" << std::endl;

	if (!alcMakeContextCurrent(p_ALCContext)) //make context current
		std::cout << "Failed to make context current" << std::endl;

	const ALCchar* name = nullptr;
	if (alcIsExtensionPresent(p_ALCDevice, "ALC_ENUMERATE_ALL_EXT"))
		name = alcGetString(p_ALCDevice, ALC_ALL_DEVICES_SPECIFIER);
	if (!name || alcGetError(p_ALCDevice) != ALC_NO_ERROR)
		name = alcGetString(p_ALCDevice, ALC_DEVICE_SPECIFIER);

#if _DEBUG
	std::cout << name << std::endl;
#endif
}

AudioDevice::~AudioDevice()  noexcept
{
	if (!alcMakeContextCurrent(nullptr))
		std::cout << "Failed to set context to nullptr" << std::endl;

	alcDestroyContext(p_ALCContext);
	if (p_ALCContext)
		std::cout << "Failed to destroy context" << std::endl;

	if (!alcCloseDevice(p_ALCDevice))
		std::cout << "Failed to close sound device" << std::endl;
}
