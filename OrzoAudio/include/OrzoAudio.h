#pragma once

#include "AudioDevice.h"
#include "AudioFormat.h"
#include "AudioSourceState.h"
#include "AudioClip.h"
#include "AudioSource.h"
#include "AudioClipInfo.h"
#include "AudioSourceInfo.h"

//AudioDevice
extern "C" __declspec(dllexport) void* CreateNewAudioDevice();
extern "C" __declspec(dllexport) void DisposeAudioDevice(AudioDevice* audioDevice);

//AudioClip
extern "C" __declspec(dllexport) void* CreateNewAudioClip();
extern "C" __declspec(dllexport) AudioClipInfo CreateNewAudioClipFromFile(const char* filePath);
extern "C" __declspec(dllexport) void DisposeAudioClip(AudioClip* audioClip);

//AudioSource
extern "C" __declspec(dllexport) AudioSourceInfo CreateNewAudioSource(AudioClip * clip);
extern "C" __declspec(dllexport) void DisposeAudioSource(AudioSource* audioSource);
extern "C" __declspec(dllexport) float AudioSourceGetPitch(AudioSource* audioSource);
extern "C" __declspec(dllexport) void AudioSourceSetPitch(AudioSource * audioSource, float pitch);
extern "C" __declspec(dllexport) float AudioSourceGetGain(AudioSource* audioSource);
extern "C" __declspec(dllexport) void AudioSourceSetGain(AudioSource * audioSource, float gain);
extern "C" __declspec(dllexport) Vector3 AudioSourceGetPosition(AudioSource* audioSource);
extern "C" __declspec(dllexport) void AudioSourceSetPosition(AudioSource * audioSource, Vector3 position);
extern "C" __declspec(dllexport) Vector3 AudioSourceGetVelocity(AudioSource* audioSource);
extern "C" __declspec(dllexport) void AudioSourceSetVelocity(AudioSource * audioSource, Vector3 velocity);
extern "C" __declspec(dllexport) bool AudioSourceGetLooping(AudioSource* audioSource);
extern "C" __declspec(dllexport) void AudioSourceSetLooping(AudioSource * audioSource, bool looping);
extern "C" __declspec(dllexport) void* AudioSourceGetClip(AudioSource* audioSource);
extern "C" __declspec(dllexport) AudioSourceState AudioSourceSetClip(AudioSource * audioSource, AudioClip * audioClip);
extern "C" __declspec(dllexport) AudioSourceState AudioSourceGetCurrentState(AudioSource * audioSource);
extern "C" __declspec(dllexport) void AudioSourceSetCurrentTime(AudioSource * audioSource, float time);
extern "C" __declspec(dllexport) float AudioSourceGetCurrentTime(AudioSource * audioSource);
extern "C" __declspec(dllexport) void AudioSourceSetCurrentFrame(AudioSource * audioSource, int frame);
extern "C" __declspec(dllexport) int AudioSourceGetCurrentFrame(AudioSource * audioSource);
extern "C" __declspec(dllexport) AudioSourceState AudioSourcePlay(AudioSource * audioSource);
extern "C" __declspec(dllexport) AudioSourceState AudioSourceStop(AudioSource * audioSource);
extern "C" __declspec(dllexport) AudioSourceState AudioSourcePause(AudioSource * audioSource);
extern "C" __declspec(dllexport) AudioSourceState AudioSourceRewind(AudioSource * audioSource);