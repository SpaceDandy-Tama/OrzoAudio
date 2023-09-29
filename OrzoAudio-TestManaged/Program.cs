using System;
using OrzoAudioNet;

namespace OrzoAudio_TestManaged
{
    internal class Program
    {
        static void Main(string[] args)
        {
            AudioDevice audioDevice = new AudioDevice();
            AudioClip audioClip = new AudioClip("Vile Tune.ogg");
            AudioSource audioSource = new AudioSource(audioClip);
            audioSource.Play();

            Console.WriteLine($"FilePath: {audioClip.FilePath}");
            Console.WriteLine($"Format: {audioClip.Format}");
            Console.WriteLine($"Channel Count: {audioClip.Channels}");
            Console.WriteLine($"Frame Count(Sample Count / Channel Count): {audioClip.Frames}");
            Console.WriteLine($"Sample Rate(Frequency): {audioClip.SampleRate}");
            Console.WriteLine($"Duration(seconds): {audioClip.Duration}");

            while(audioSource.State == AudioSourceState.Playing)
                Console.Write($"\rCurrent Time: {audioSource.Time} - Current Frame: {audioSource.Frame}");

            Console.Read();

            audioClip.Dispose();
            audioSource.Dispose();
            audioDevice.Dispose();
        }
    }
}
