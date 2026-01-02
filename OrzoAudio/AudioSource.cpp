#include <iostream>

#include "AudioSource.h"
#include <AL/alc.h>

AudioSource::AudioSource(AudioClip* clip)
{
	alGenSources(1, &p_Source);
	SetPitch(1.0f);
	SetGain(1.0f);
	SetPosition(0, 0, 0);
	SetVelocity(0, 0, 0);
	SetLooping(false);
	SetClip(clip);
	GetCurrentState();
}

AudioSource::~AudioSource() noexcept
{
#if _DEBUG
	std::cout << "Destroying AudioSource " << this << std::endl;
#endif

	if (p_Source == 0)
		return;

	if (alcGetCurrentContext() == nullptr)
		return;

	alSourceStop(p_Source);
	alSourcei(p_Source, AL_BUFFER, 0);
	alDeleteSources(1, &p_Source);
	p_Source = 0;
}


void AudioSource::SetPitch(float pitch)
{
	p_Pitch = pitch;
	alSourcef(p_Source, AL_PITCH, p_Pitch);
}

float AudioSource::Getpitch()
{
	return p_Pitch;
}

void AudioSource::SetGain(float gain)
{
	p_Gain = gain;
	alSourcef(p_Source, AL_GAIN, p_Gain);
}

float AudioSource::GetGain()
{
	return p_Gain;
}

void AudioSource::SetPosition(float x, float y, float z)
{
	p_Position.x = x;
	p_Position.y = y;
	p_Position.z = z;
	alSource3f(p_Source, AL_POSITION, p_Position.x, p_Position.y, p_Position.z);
}

void AudioSource::SetPosition(Vector3& position)
{
	SetPosition(position.x, position.y, position.z);
}

Vector3 AudioSource::GetPosition()
{
	return p_Position;
}

void AudioSource::SetVelocity(float x, float y, float z)
{
	p_Velocity.x = x;
	p_Velocity.y = y;
	p_Velocity.z = z;
	alSource3f(p_Source, AL_VELOCITY, p_Velocity.x, p_Velocity.y, p_Velocity.z);
}

void AudioSource::SetVelocity(Vector3& velocity)
{
	SetVelocity(velocity.x, velocity.y, velocity.z);
}

Vector3 AudioSource::GetVelocity()
{
	return p_Velocity;
}

void AudioSource::SetLooping(bool looping)
{
	p_Looping = looping;
	alSourcei(p_Source, AL_LOOPING, p_Looping);
}

bool AudioSource::GetLooping()
{
	return p_Looping;
}

AudioSourceState AudioSource::SetClip(AudioClip* clip)
{
	Stop();
	m_Clip = clip;
	alSourcei(p_Source, AL_BUFFER, (ALint)(m_Clip ? m_Clip->GetBufferID() : 0));
	return GetCurrentState();
}

AudioClip* AudioSource::GetClip()
{
	return m_Clip;
}

AudioSourceState AudioSource::GetCurrentState()
{
	int result;
	alGetSourcei(p_Source, AL_SOURCE_STATE, &result);
	return (AudioSourceState)result;
}

void AudioSource::SetCurrentTime(float time)
{
	alSourcef(p_Source, AL_SEC_OFFSET, time);
}

float AudioSource::GetCurrentTime()
{
	float result;
	alGetSourcef(p_Source, AL_SEC_OFFSET, &result);
	return result;
}

void AudioSource::SetCurrentFrame(int frame)
{
	alSourcef(p_Source, AL_SAMPLE_OFFSET, frame);
}

int AudioSource::GetCurrentFrame()
{
	int result;
	alGetSourcei(p_Source, AL_SAMPLE_OFFSET, &result);
	return result;
}

AudioSourceState AudioSource::Play()
{
	if (m_Clip == nullptr || p_Source == 0)
		return GetCurrentState();

	alSourcePlay(p_Source);
	return GetCurrentState();
}

AudioSourceState AudioSource::Play(AudioClip* clip)
{
	if (clip != m_Clip)
		SetClip(clip);

	return Play();
}

AudioSourceState AudioSource::Stop()
{
	if (p_Source == 0)
		return GetCurrentState();

	alSourceStop(p_Source);
	return GetCurrentState();
}

AudioSourceState AudioSource::Pause()
{
	if (p_Source == 0)
		return GetCurrentState();

	alSourcePause(p_Source);
	return GetCurrentState();
}

AudioSourceState AudioSource::Rewind()
{
	if (p_Source == 0)
		return GetCurrentState();

	alSourceRewind(p_Source);
	return GetCurrentState();
}
