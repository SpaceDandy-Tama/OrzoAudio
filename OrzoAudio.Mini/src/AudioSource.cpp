#include <iostream>
#include <cstdint>

#include "../include/AudioSource.h"

void AudioSource::InitializeSound()
{
    if (!AudioDevice::IsInitialized() || m_Clip == nullptr)
        return;

    bool wasInitialized = false;
    if (m_Initialized)
    {
        ma_sound_uninit(&m_MaSound);
        m_Initialized = false;
        wasInitialized = true;
    }

    unsigned int flags = (m_Spatial ? 0 : MA_SOUND_FLAG_NO_SPATIALIZATION) | (m_Doppler ? 0 : MA_SOUND_FLAG_NO_PITCH);
    ma_sound_group* group = m_Bus ? &m_Bus->m_Group : nullptr;

    m_Initialized = ma_sound_init_from_data_source(AudioDevice::GetEngine(), m_Clip->m_Buffer, flags, group, &m_MaSound) == MA_SUCCESS;

    if (!m_Initialized)
    {
        std::cout << "FAILED AudioSource Initialization";
        return;
    }

    ma_sound_set_pan_mode(&m_MaSound, ma_pan_mode_balance);
    ma_sound_set_min_gain(&m_MaSound, 0.0f);
    ma_sound_set_max_gain(&m_MaSound, 2.0f);

    if (wasInitialized)
    {
        SetPan(m_Pan);
        SetPitch(m_Pitch);
        SetGain(m_Gain);
        SetPosition(m_Position);
        SetVelocity(m_Velocity);
        SetDirection(m_Direction);
        SetLooping(m_Looping);
    }
}

AudioSource::AudioSource(AudioClip* clip, AudioBus* bus, bool spatial, bool doppler) : m_Bus{ bus }, m_Clip{ clip }, m_Spatial { spatial }, m_Doppler { doppler }
{
    AudioSource::InitializeSound();
}
AudioSource::~AudioSource()
{
    if (m_Initialized)
        ma_sound_uninit(&m_MaSound);
}

void AudioSource::SetSpatial(bool spatial)
{
    ma_sound_set_spatialization_enabled(&m_MaSound, m_Spatial = spatial);
}
bool AudioSource::IsSpatial() const
{
    return m_Spatial;
}
bool AudioSource::IsDopplerEnabled() const
{
    return m_Doppler;
}

void AudioSource::SetPan(float pan)
{
    ma_sound_set_pan(&m_MaSound, m_Pan = pan);
}
float AudioSource::GetPan() const
{
    //return ma_sound_get_pan(&m_MaSound);
    return m_Pan;
}

void AudioSource::SetPitch(float pitch)
{
    ma_sound_set_pitch(&m_MaSound, m_Pitch = pitch);
}
float AudioSource::GetPitch() const
{
    //return ma_sound_get_pitch(&m_MaSound);
    return m_Pitch;
}

void AudioSource::SetGain(float gain)
{
    ma_sound_set_volume(&m_MaSound, m_Gain = gain);
}
float AudioSource::GetGain() const
{
    //return ma_sound_get_volume(&m_MaSound);
    return m_Gain;
}

void AudioSource::SetPosition(float x, float y, float z)
{
    Vector3 vector = Vector3{ x, y, z };
    SetPosition(vector);
}
void AudioSource::SetPosition(Vector3& position)
{
    m_Position = position;
    ma_sound_set_position(&m_MaSound, m_Position.x, m_Position.y, -m_Position.z);
}
Vector3 AudioSource::GetPosition() const
{
    //ma_vec3f mavec3f = ma_sound_get_position(&m_MaSound);
    //return Vector3{ mavec3f.x, mavec3f.y, -mavec3f.z };
    return m_Position;
}

void AudioSource::SetVelocity(float x, float y, float z)
{
    Vector3 vector = Vector3{ x, y, z };
    SetVelocity(vector);
    
}
void AudioSource::SetVelocity(Vector3& velocity)
{
    m_Velocity = velocity;
    ma_sound_set_velocity(&m_MaSound, m_Velocity.x, m_Velocity.y, -m_Velocity.z);
}
Vector3 AudioSource::GetVelocity() const
{
    //ma_vec3f mavec3f = ma_sound_get_velocity(&m_MaSound);
    //return Vector3{ mavec3f.x, mavec3f.y, -mavec3f.z };
    return m_Velocity;
}

void AudioSource::SetDirection(float x, float y, float z)
{
    Vector3 vector = Vector3{ x, y, z };
    SetDirection(vector);
    
}
void AudioSource::SetDirection(Vector3& direction)
{
    m_Direction = direction;
    ma_sound_set_direction(&m_MaSound, m_Direction.x, m_Direction.y, -m_Direction.z);
}
Vector3 AudioSource::GetDirection() const
{
    //ma_vec3f mavec3f = ma_sound_get_direction(&m_MaSound);
    //return Vector3{ mavec3f.x, mavec3f.y, -mavec3f.z };
    return m_Direction;
}

void AudioSource::SetLooping(bool looping)
{
    ma_sound_set_looping(&m_MaSound, m_Looping = looping);
}
bool AudioSource::GetLooping() const
{
    //return ma_sound_is_looping(&m_MaSound);
    return m_Looping;
}

void AudioSource::SetClip(AudioClip* clip)
{
    if (clip == nullptr || m_Clip == clip)
        return;

    Stop();

    m_Clip = clip;

    InitializeSound();
}
AudioClip* AudioSource::GetClip() const
{
    return m_Clip;
}
void AudioSource::SetBus(AudioBus* bus)
{
    if (m_Bus == bus)
        return;

    Stop();

    m_Bus = bus;

    InitializeSound();
}
AudioBus* AudioSource::GetBus() const
{
    return m_Bus;
}
void AudioSource::SetClipAndBus(AudioClip* clip, AudioBus* bus)
{
    if (clip == nullptr || (m_Clip == clip && m_Bus == bus))
        return;

    Stop();

    m_Clip = clip;
    m_Bus = bus;

    InitializeSound();
}

void AudioSource::SetPlaybackTime(double time)
{
    double duration = m_Clip->GetDuration();

    if (time < 0.0f)
        time = 0.0f;
    else if (time > duration)
        time = duration;

    uint64_t frame = static_cast<uint64_t>(time * m_Clip->GetSampleRate());
    SetCurrentFrame(frame);
}

double AudioSource::GetPlaybackTime() const
{
    return GetCurrentFrame() / static_cast<double>(m_Clip->GetSampleRate());
}

void AudioSource::SetCurrentFrame(uint64_t frame)
{
    ma_sound_seek_to_pcm_frame(&m_MaSound, frame);
}

uint64_t AudioSource::GetCurrentFrame() const
{
    uint64_t currentFrame = 0;
    ma_sound_get_cursor_in_pcm_frames(&m_MaSound, &currentFrame);
    return currentFrame;
}

void AudioSource::Play()
{
    ma_sound_start(&m_MaSound);
}

void AudioSource::Play(AudioClip* clip)
{
    SetClip(clip);
    Play();
}

bool AudioSource::IsPlaying() const
{
    return ma_sound_is_playing(&m_MaSound);
}

void AudioSource::Stop()
{
    Pause();
    Rewind();
}

bool AudioSource::HasReachedEnd() const
{
    return ma_sound_at_end(&m_MaSound);
}

void AudioSource::Pause()
{
    ma_sound_stop(&m_MaSound);
}

void AudioSource::Rewind()
{
    SetCurrentFrame(0);
}
