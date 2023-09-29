#pragma once

#include "Vector3.h"

struct __declspec(dllexport) AudioSourceInfo
{
public:
	void* AudioSourcePointer = nullptr;
    float Pitch;
    float Gain;
    Vector3 Position;
    Vector3 Velocity;
    int Looping;
    void* ClipPointer;
    AudioSourceState State;
};