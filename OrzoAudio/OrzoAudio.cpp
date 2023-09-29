#include "OrzoAudio.h"

void* CreateNewAudioDevice()
{
	return new AudioDevice();
}

void DisposeAudioDevice(AudioDevice* audioDevice)
{
	audioDevice->~AudioDevice();
}

void* CreateNewAudioClip()
{
	return new AudioClip();
}

AudioClipInfo CreateNewAudioClipFromFile(const char* filePath)
{
	AudioClipInfo info;
	AudioClip* audioClip = AudioClip::FromFile(filePath);
	info.AudioClipPointer = audioClip;
	info.Format = audioClip->GetFormat();
	info.Channels = audioClip->GetChannelCount();
	info.Frames = audioClip->GetFrameCount();
	info.SampleRate = audioClip->GetSampleRate();
	info.Duration = audioClip->GetDuration();
	return info;
}

void DisposeAudioClip(AudioClip* audioClip)
{
	audioClip->~AudioClip();
}

AudioSourceInfo CreateNewAudioSource(AudioClip* clip)
{
	AudioSourceInfo info;
	AudioSource* audioSource = new AudioSource(clip);
	info.AudioSourcePointer = audioSource;
	info.Pitch = audioSource->Getpitch();
	info.Gain = audioSource->GetGain();
	info.Position = audioSource->GetPosition();
	info.Velocity = audioSource->GetVelocity();
	info.Looping = audioSource->GetLooping() ? 255 : 0;
	info.ClipPointer = audioSource->GetClip();
	info.State = audioSource->GetCurrentState();
	return info;
}

void DisposeAudioSource(AudioSource* audioSource)
{
	audioSource->~AudioSource();
}

float AudioSourceGetPitch(AudioSource* audioSource)
{
	return audioSource->Getpitch();
}

void AudioSourceSetPitch(AudioSource* audioSource, float pitch)
{
	audioSource->SetPitch(pitch);
}

float AudioSourceGetGain(AudioSource* audioSource)
{
	return audioSource->GetGain();
}

void AudioSourceSetGain(AudioSource* audioSource, float gain)
{
	audioSource->SetGain(gain);
}

Vector3 AudioSourceGetPosition(AudioSource* audioSource)
{
	return audioSource->GetPosition();
}

void AudioSourceSetPosition(AudioSource* audioSource, Vector3 position)
{
	audioSource->SetPosition(position);
}

Vector3 AudioSourceGetVelocity(AudioSource* audioSource)
{
	return audioSource->GetVelocity();
}

void AudioSourceSetVelocity(AudioSource* audioSource, Vector3 velocity)
{
	audioSource->SetVelocity(velocity);
}

bool AudioSourceGetLooping(AudioSource* audioSource)
{
	return audioSource->GetLooping();
}

void AudioSourceSetLooping(AudioSource* audioSource, bool looping)
{
	audioSource->SetLooping(looping);
}

void* AudioSourceGetClip(AudioSource* audioSource)
{
	return audioSource->GetClip();
}

AudioSourceState AudioSourceSetClip(AudioSource* audioSource, AudioClip* audioClip)
{
	return audioSource->SetClip(audioClip);
}

AudioSourceState AudioSourceGetCurrentState(AudioSource* audioSource)
{
	return audioSource->GetCurrentState();
}

void AudioSourceSetCurrentTime(AudioSource* audioSource, float time)
{
	audioSource->SetCurrentTime(time);
}

float AudioSourceGetCurrentTime(AudioSource* audioSource)
{
	return audioSource->GetCurrentTime();
}

void AudioSourceSetCurrentFrame(AudioSource* audioSource, int frame)
{
	audioSource->SetCurrentFrame(frame);
}

int AudioSourceGetCurrentFrame(AudioSource* audioSource)
{
	return audioSource->GetCurrentFrame();
}

AudioSourceState AudioSourcePlay(AudioSource* audioSource)
{
	return audioSource->Play();
}

AudioSourceState AudioSourceStop(AudioSource* audioSource)
{
	return audioSource->Stop();
}

AudioSourceState AudioSourcePause(AudioSource* audioSource)
{
	return audioSource->Pause();
}

AudioSourceState AudioSourceRewind(AudioSource* audioSource)
{
	return audioSource->Rewind();
}
