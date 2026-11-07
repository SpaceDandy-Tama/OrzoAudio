#include <cmath>
#include <iostream>
#include <string>
#include <iostream>
#include <chrono>
#include <thread>
#include <conio.h>
#include <csignal>
using namespace std::chrono;

#include "AudioFormat.h"
#include "AudioDevice.h"
#include "AudioClip.h"
#include "AudioBus.h"
#include "AudioSource.h"

std::atomic<bool> running = true;
void SignalHandler(int signal)
{
    if (signal == SIGINT)
        running.store(false);
}

void DisplayDirection(const Vector3& position)
{
    float angle = atan2f(position.z, position.x);

    const char* arrows[] = {"6", "3", "2", "1", "4", "7", "8", "9"};

    int index = static_cast<int>(roundf(angle / (3.14159265f / 4.0f)));

    index = (index + 8) % 8;

    std::cout << "\rDirection: " << arrows[index] << "   " << std::flush;
}

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cout << "No argument" << std::endl;
        return -1;
    }
    else if (argc > 2)
    {
        std::cout << "Too many arguments" << std::endl;
        return -1;
    }
    std::string filePath = argv[1];
    std::cout << filePath << std::endl;

    if (!AudioDevice::Initialize())
    {
        std::cout << "Couldn't start AudioDevice";
        return -1;
    }

    AudioClip* clip = AudioClip::FromFile(filePath);
    if (clip)
    {
        std::cout << "FileName:   " << clip->GetFilePath() << std::endl;
        std::cout << "Format:     " << clip->GetFormat() << std::endl;
        std::cout << "Channels:   " << clip->GetChannelCount() << std::endl;
        std::cout << "Frames:     " << clip->GetFrameCount() << std::endl;
        std::cout << "SampleRate: " << clip->GetSampleRate() << std::endl;
        std::cout << "Duration:   " << clip->GetDuration() << std::endl;
        std::cout << std::endl;
    }
    else
    {
        std::cout << "Couldn't Open AudioClip";
        return -1;
    }

    AudioBus* bus = new AudioBus("Music");

    AudioSource* source = new AudioSource(clip, bus);
    source->SetLooping(true);
    source->Play();

    std::signal(SIGINT, SignalHandler);
    auto nextUpdate = steady_clock::now();
    float time = 0.0f;
    while (running.load())
    {
        nextUpdate += milliseconds(20);
        //
        
        // Allow Enter to exit.
        if (_kbhit() && _getch() == '\r')
        {
            running.store(false);
            break;
        }

        float radius = 5.0f;
        float speed = 1.0f;

        Vector3 position{radius * cosf(time * speed), 0.0f, radius * sinf(time * speed)};

        source->SetPosition(position.x, position.y, position.z);
        DisplayDirection(position);
        time += 0.02f;

        //
        std::this_thread::sleep_until(nextUpdate);
    }

    source->Stop();
    delete source;
    delete bus;
    delete clip;
    AudioDevice::Destroy();

    return 0;
}