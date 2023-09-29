#pragma once

#include <AL/al.h>

#include "AudioClip.h"
#include "Vector3.h"
#include "AudioSourceState.h"

class __declspec(dllexport) AudioSource
{
public:
	AudioSource(AudioClip* clip);
	~AudioSource();

	void SetPitch(float pitch);
	float Getpitch();
	void SetGain(float gain);
	float GetGain();
	void SetPosition(float x, float y, float z);
	void SetPosition(Vector3& position);
	Vector3 GetPosition();
	void SetVelocity(float x, float y, float z);
	void SetVelocity(Vector3& velocity);
	Vector3 GetVelocity();
	void SetLooping(bool looping);
	bool GetLooping();
	AudioSourceState SetClip(AudioClip* clip);
	AudioClip* GetClip();
	
	AudioSourceState GetCurrentState();
	void SetCurrentTime(float time);
	float GetCurrentTime();
	void SetCurrentFrame(int frame);
	int GetCurrentFrame();
	AudioSourceState Play();
	AudioSourceState Play(AudioClip* clip);
	AudioSourceState Stop();
	AudioSourceState Pause();
	AudioSourceState Rewind();

private:
	ALuint p_Source;
	float p_Pitch;
	float p_Gain;
	Vector3 p_Position;
	Vector3 p_Velocity;
	bool p_Looping;
	AudioClip* m_Clip;
};