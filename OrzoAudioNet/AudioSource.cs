using System.Runtime.InteropServices;
using System;

namespace OrzoAudioNet
{
    [StructLayout(LayoutKind.Sequential)]
    public struct Vector3
    {
        public float x;
        public float y;
        public float z;
    }

    [StructLayout(LayoutKind.Sequential)]
    internal struct AudioSourceInfo
    {
        public IntPtr AudioSourcePointer;
        public float Pitch;
        public float Gain;
        public Vector3 Position;
        public Vector3 Velocity;
        public int Looping;
        public IntPtr ClipPointer;
        public AudioSourceState State;
    };

    public class AudioSource : IDisposable
    {
        [DllImport(OrzoAudio.DLLPath)]
        internal static extern AudioSourceInfo CreateNewAudioSource(IntPtr audioClip);

        [DllImport(OrzoAudio.DLLPath)]
        internal static extern IntPtr DisposeAudioSource(IntPtr audioSource);

        [DllImport(OrzoAudio.DLLPath)]
        internal static extern float AudioSourceGetPitch(IntPtr audioSource);
        [DllImport(OrzoAudio.DLLPath)]
        internal static extern void AudioSourceSetPitch(IntPtr audioSource, float pitch);
        [DllImport(OrzoAudio.DLLPath)]
        internal static extern float AudioSourceGetGain(IntPtr audioSource);
        [DllImport(OrzoAudio.DLLPath)]
        internal static extern void AudioSourceSetGain(IntPtr audioSource, float gain);
        [DllImport(OrzoAudio.DLLPath)]
        internal static extern Vector3 AudioSourceGetposition(IntPtr audioSource);
        [DllImport(OrzoAudio.DLLPath)]
        internal static extern void AudioSourceSetPosition(IntPtr audioSource, Vector3 position);
        [DllImport(OrzoAudio.DLLPath)]
        internal static extern Vector3 AudioSourceGetVelocity(IntPtr audioSource);
        [DllImport(OrzoAudio.DLLPath)]
        internal static extern void AudioSourceSetVelocity(IntPtr audioSource, Vector3 velocity);
        [DllImport(OrzoAudio.DLLPath)]
        internal static extern bool AudioSourceGetLooping(IntPtr audioSource);
        [DllImport(OrzoAudio.DLLPath)]
        internal static extern void AudioSourceSetLooping(IntPtr audioSource, bool looping);
        [DllImport(OrzoAudio.DLLPath)]
        internal static extern IntPtr AudioSourceGetClip(IntPtr audioSource);
        [DllImport(OrzoAudio.DLLPath)]
        internal static extern AudioSourceState AudioSourceSetClip(IntPtr audioSource, IntPtr audioClip);

        [DllImport(OrzoAudio.DLLPath)]
        internal static extern AudioSourceState AudioSourceGetCurrentState(IntPtr audioSource);
        [DllImport(OrzoAudio.DLLPath)]
        internal static extern void AudioSourceSetCurrentTime(IntPtr audioSource, float time);
        [DllImport(OrzoAudio.DLLPath)]
        internal static extern float AudioSourceGetCurrentTime(IntPtr audioSource);
        [DllImport(OrzoAudio.DLLPath)]
        internal static extern void AudioSourceSetCurrentFrame(IntPtr audioSource, int frame);
        [DllImport(OrzoAudio.DLLPath)]
        internal static extern int AudioSourceGetCurrentFrame(IntPtr audioSource);
        [DllImport(OrzoAudio.DLLPath)]
        internal static extern AudioSourceState AudioSourcePlay(IntPtr audioSource);
        [DllImport(OrzoAudio.DLLPath)]
        internal static extern AudioSourceState AudioSourceStop(IntPtr audioSource);
        [DllImport(OrzoAudio.DLLPath)]
        internal static extern AudioSourceState AudioSourcePause(IntPtr audioSource);
        [DllImport(OrzoAudio.DLLPath)]
        internal static extern AudioSourceState AudioSourceRewind(IntPtr audioSource);

        internal IntPtr UnmanagedHandle = IntPtr.Zero;

        private float pitch;
        private float gain;
        private Vector3 position;
        private Vector3 velocity;
        private bool looping;
        private AudioClip clip;
        private AudioSourceState state;

        public float Pitch
        {
            get => pitch;
            set
            {
                AudioSourceSetPitch(UnmanagedHandle, value);
                pitch = value;
            }
        }
        public float Gain
        {
            get => gain;
            set
            {
                AudioSourceSetGain(UnmanagedHandle, value);
                gain = value;
            }
        }
        public Vector3 Position
        {
            get => position;
            set
            {
                AudioSourceSetPosition(UnmanagedHandle, value);
                position = value;
            }
        }
        public Vector3 Velocity
        {
            get => velocity;
            set
            {
                AudioSourceSetVelocity(UnmanagedHandle, value);
                velocity = value;
            }
        }
        public bool Looping
        {
            get => looping;
            set
            {
                AudioSourceSetLooping(UnmanagedHandle, value);
                looping = value;
            }
        }
        public AudioClip Clip
        {
            get => clip;
            set
            {
                AudioSourceSetClip(UnmanagedHandle, clip.UnmanagedHandle);
                clip = value;
            }
        }
        public AudioSourceState State => state = AudioSourceGetCurrentState(UnmanagedHandle);
        public double ClipDuration => clip != null ? clip.Duration : 0.0;
        public long ClipFrames => clip != null ? clip.Frames : 0;
        public float Time
        {
            get => AudioSourceGetCurrentTime(UnmanagedHandle);
            set
            {
                float clampedValue = value;
                if (value < 0)
                    clampedValue = 0.0f;
                else if (value > ClipDuration)
                    clampedValue = (float)ClipDuration;
                AudioSourceSetCurrentTime(UnmanagedHandle, clampedValue);
            }
        }
        public int Frame
        {
            get => AudioSourceGetCurrentFrame(UnmanagedHandle);
            set
            {
                int clampedValue = value;
                if (clampedValue < 0)
                    clampedValue = 0;
                else if (clampedValue > ClipFrames)
                    clampedValue = (int)ClipFrames;
                AudioSourceSetCurrentFrame(UnmanagedHandle, clampedValue);
            }
        }

        public AudioSource(AudioClip audioClip)
        {
            AudioSourceInfo info = CreateNewAudioSource(audioClip.UnmanagedHandle);
            UnmanagedHandle = info.AudioSourcePointer;
            pitch = info.Pitch;
            gain = info.Gain;
            position = info.Position;
            velocity = info.Velocity;
            looping = info.Looping > 0 ? true : false;
            clip = audioClip;
            state = info.State;
        }

        public void Play()
        {
            if (clip != null)
                state = AudioSourcePlay(UnmanagedHandle);
            state = AudioSourceGetCurrentState(UnmanagedHandle);
        }
        public void Play(AudioClip clip)
        {
            if (Clip != clip)
                Clip = clip;
            Play();
        }
        public void Stop() => state = AudioSourceStop(UnmanagedHandle);
        public void Pause() => state = AudioSourcePause(UnmanagedHandle);
        public void Rewind() => state = AudioSourceRewind(UnmanagedHandle);

        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }

        protected virtual void Dispose(bool disposing)
        {
            if (UnmanagedHandle != IntPtr.Zero)
            {
                if (disposing)
                {
                    DisposeAudioSource(UnmanagedHandle);
                    UnmanagedHandle = IntPtr.Zero;
                }

#if DEBUG
                Console.WriteLine("AudioSource Disposed");
#endif
            }
        }
    }
}
