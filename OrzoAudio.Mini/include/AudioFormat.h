#pragma once

#include "miniaudio/miniaudio.h"

enum __declspec(dllexport) AudioFormat : int
{
	OF_None = 0,
	OF_8BitUnsigned = 1,
	OF_16BitSigned = 2,
	OF_24BitSigned = 3,
	OF_32BitSigned = 4,
	OF_32BitFloat = 5,
};