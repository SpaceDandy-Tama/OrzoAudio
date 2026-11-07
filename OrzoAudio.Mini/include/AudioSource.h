#pragma once

#include <cstdint>

#include "AudioDevice.h"
#include "AudioBus.h"
#include "AudioClip.h"
#include "Vector.h"

class __declspec(dllexport) AudioSource
{
public:
	AudioSource(AudioClip* clip, AudioBus* bus = nullptr, bool spatial = true, bool doppler = true);
	~AudioSource();

	void SetSpatial(bool spatial);
	bool IsSpatial() const;
	bool IsDopplerEnabled() const;

	void SetPan(float pan);
	float GetPan() const;
	void SetPitch(float pitch);
	float GetPitch() const;
	void SetGain(float gain);
	float GetGain() const;
	void SetPosition(float x, float y, float z);
	void SetPosition(Vector3& position);
	Vector3 GetPosition() const;
	void SetVelocity(float x, float y, float z);
	void SetVelocity(Vector3& velocity);
	Vector3 GetVelocity() const;
	void SetDirection(float x, float y, float z);
	void SetDirection(Vector3& velocity);
	Vector3 GetDirection() const;
	void SetLooping(bool looping);
	bool GetLooping() const;
	void SetClip(AudioClip* clip);
	AudioClip* GetClip() const;
	void SetBus(AudioBus* bus);
	AudioBus* GetBus() const;
	void SetClipAndBus(AudioClip* clip, AudioBus* bus);

	void SetPlaybackTime(double time);
	double GetPlaybackTime() const;
	void SetCurrentFrame(uint64_t frame);
	uint64_t GetCurrentFrame() const;
	void Play();
	void Play(AudioClip* clip);
	bool IsPlaying() const;
	void Stop();
	bool HasReachedEnd() const;
	void Pause();
	void Rewind();

private:
	void InitializeSound();
	ma_sound m_MaSound{};
	bool m_Initialized = false;

	AudioBus* m_Bus;
	AudioClip* m_Clip;
	bool m_Spatial;
	bool m_Doppler;

	float m_Pan = 0.0f;
	float m_Pitch = 1.0f;
	float m_Gain = 1.0f;
	Vector3 m_Position = Vector3{ 0, 0, 0 };
	Vector3 m_Velocity = Vector3{ 0, 0, 0 };
	Vector3 m_Direction = Vector3{ 0, 0, 1 };
	bool m_Looping = false;
};