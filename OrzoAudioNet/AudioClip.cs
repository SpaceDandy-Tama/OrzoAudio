using System;
using System.Runtime.InteropServices;

namespace OrzoAudioNet
{
    [StructLayout(LayoutKind.Sequential)]
    internal struct AudioClipInfo
    {
        public IntPtr AudioClipPointer;
        public AudioFormat Format;
        public int Channels;
        public long Frames;
        public int SampleRate;
        public double Duration;
    };

    public class AudioClip : IDisposable
    {
        [DllImport(OrzoAudio.DLLPath)]
        internal static extern IntPtr CreateNewAudioClip();
        [DllImport(OrzoAudio.DLLPath)]
        internal static extern AudioClipInfo CreateNewAudioClipFromFile(string filePath);

        [DllImport(OrzoAudio.DLLPath)]
        internal static extern IntPtr DisposeAudioClip(IntPtr audioClip);

        internal IntPtr UnmanagedHandle = IntPtr.Zero;

        public string FilePath { get; private set; }
        public AudioFormat Format { get; private set; }
        public int Channels { get; private set; }
        public long Frames { get; private set; }
        public int SampleRate { get; private set; }
        public double Duration { get; private set; }

        public AudioClip()
        {
            UnmanagedHandle = CreateNewAudioClip();
            FilePath = null;
            Format = AudioFormat.None;
            Channels = 0;
            Frames = 0;
            SampleRate = 0;
            Duration = 0;
        }
        public AudioClip(string filePath)
        {
            AudioClipInfo info = CreateNewAudioClipFromFile(filePath);
            UnmanagedHandle = info.AudioClipPointer;
            FilePath = filePath;
            Format = info.Format;
            Channels = info.Channels;
            Frames = info.Frames;
            SampleRate = info.SampleRate;
            Duration = info.Duration;
        }

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
                    DisposeAudioClip(UnmanagedHandle);
                    UnmanagedHandle = IntPtr.Zero;
                }

#if DEBUG
                Console.WriteLine("AudioClip Disposed");
#endif
            }
        }
    }
}
