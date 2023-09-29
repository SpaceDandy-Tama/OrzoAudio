using System;
using System.Runtime.InteropServices;

namespace OrzoAudioNet
{
    public class AudioDevice : IDisposable
    {
        [DllImport(OrzoAudio.DLLPath)]
        internal static extern IntPtr CreateNewAudioDevice();

        [DllImport(OrzoAudio.DLLPath)]
        internal static extern IntPtr DisposeAudioDevice(IntPtr audioDevice);

        internal IntPtr UnmanagedHandle = IntPtr.Zero;

        public AudioDevice()
        {
            UnmanagedHandle = CreateNewAudioDevice();
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
                    DisposeAudioDevice(UnmanagedHandle);
                    UnmanagedHandle = IntPtr.Zero;
                }

#if DEBUG
                Console.WriteLine("AudioDevice Disposed");
#endif
            }
        }
    }
}
