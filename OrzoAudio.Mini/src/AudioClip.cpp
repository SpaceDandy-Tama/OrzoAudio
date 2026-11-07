#include <iostream>

//#include "../include/miniaudio/decoders/libvorbis/miniaudio_libvorbis.h"

#include "../include/stb/stb_vorbis.h"

#include "../include/AudioClip.h"

AudioClip::AudioClip()
{
    m_Buffer = nullptr;
    m_Format = AudioFormat::OF_None;
    m_Channels = 0;
    m_Frames = 0;
    m_SampleRate = 0;
    m_Duration = 0;
}

AudioClip::~AudioClip()
{
    if (m_Buffer != nullptr)
    {
        ma_audio_buffer_uninit_and_free(m_Buffer);
        m_Buffer = nullptr;
    }
}

AudioClip* AudioClip::FromFile(const std::string& filePath)
{
    if (!AudioDevice::IsInitialized())
        return nullptr;

    AudioFormat mixingFormat = AudioDevice::GetMixingFormat();
    unsigned int outputSampleRate = AudioDevice::GetOutputSampleRate();

    //Set up AudioClip
    AudioClip* clip = new AudioClip();
    clip->m_FilePath = filePath;

    bool isOGG = filePath.size() >= 4 && (filePath.compare(filePath.size() - 4, 4, ".ogg") == 0 || filePath.compare(filePath.size() - 4, 4, ".OGG") == 0);
    if (isOGG)
    {
        int error = 0;
        stb_vorbis* vorbis = stb_vorbis_open_filename(filePath.c_str(), &error, nullptr);

        if (vorbis == nullptr)
        {
            std::cout << "Failed to Open, or not a Vorbis stream. Error: " << error << std::endl;
            delete clip;
            return nullptr;
        }

        stb_vorbis_info info = stb_vorbis_get_info(vorbis);
        clip->m_Channels = info.channels;
        clip->m_Format = mixingFormat;
        clip->m_SampleRate = outputSampleRate;
        clip->m_Frames = stb_vorbis_stream_length_in_samples(vorbis);

        if (clip->m_Frames == 0)
        {
            delete clip;
            stb_vorbis_close(vorbis);
            return nullptr;
        }

        size_t bytesPerFrame = sizeof(float) * clip->m_Channels;
        size_t dataSize = static_cast<size_t>(clip->m_Frames) * bytesPerFrame;
        float* pcmDataIn = static_cast<float*>(malloc(dataSize));
        if (pcmDataIn == nullptr)
        {
            delete clip;
            stb_vorbis_close(vorbis);
            return nullptr;
        }
        int numFloats = static_cast<int>(clip->m_Frames) * clip->m_Channels;
        uint64_t framesRead = stb_vorbis_get_samples_float_interleaved(vorbis, clip->m_Channels, pcmDataIn, numFloats);
        stb_vorbis_close(vorbis);
        if (framesRead <= 0)
        {
            delete clip;
            free(pcmDataIn);
            return nullptr;
        }
        clip->m_Frames = framesRead;

        ma_format inputFormat = ma_format_f32;
        ma_format outputFormat = AudioDevice::FromAudioFormat(clip->m_Format);
        ma_data_converter_config dataConverterConfig = ma_data_converter_config_init(
            inputFormat,
            outputFormat,
            clip->m_Channels,
            clip->m_Channels,
            info.sample_rate,
            clip->m_SampleRate);

        ma_data_converter converter;
        if (ma_data_converter_init(&dataConverterConfig, NULL, &converter) != MA_SUCCESS)
        {
            delete clip;
            free(pcmDataIn);
            return nullptr;
        }
        uint64_t frameCountIn = clip->m_Frames;
        uint64_t frameCountOut = 0;
        if (ma_data_converter_get_expected_output_frame_count(&converter, frameCountIn, &frameCountOut) != MA_SUCCESS || frameCountOut == 0)
        {
            delete clip;
            free(pcmDataIn);
            ma_data_converter_uninit(&converter, nullptr);
            return nullptr;
        }
        bytesPerFrame = ma_get_bytes_per_frame(outputFormat, clip->m_Channels);
        dataSize = static_cast<size_t>(frameCountOut) * bytesPerFrame;
        void* pcmDataOut = malloc(dataSize);
        if (pcmDataOut == nullptr)
        {
            delete clip;
            free(pcmDataIn);
            ma_data_converter_uninit(&converter, nullptr);
            return nullptr;
        }
        if (ma_data_converter_process_pcm_frames(&converter, pcmDataIn, &frameCountIn, pcmDataOut, &frameCountOut) != MA_SUCCESS)
        {
            delete clip;
            free(pcmDataOut);
            free(pcmDataIn);
            ma_data_converter_uninit(&converter, nullptr);
            return nullptr;
        }
        ma_data_converter_uninit(&converter, nullptr);
        free(pcmDataIn);
        if (frameCountIn != clip->m_Frames)
        {
            delete clip;
            free(pcmDataOut);
            return nullptr;
        }
        clip->m_Frames = frameCountOut;
        clip->m_Duration = static_cast<double>(clip->m_Frames) / clip->m_SampleRate;

        ma_audio_buffer_config config = ma_audio_buffer_config_init(AudioDevice::FromAudioFormat(clip->m_Format), clip->m_Channels, clip->m_Frames, pcmDataOut, nullptr);
        ma_result result = ma_audio_buffer_alloc_and_init(&config, &clip->m_Buffer);
        free(pcmDataOut);
        if (result != MA_SUCCESS)
        {
            delete clip;
            return nullptr;
        }
    }
    else
    {
        //Set up decoder and decode
        ma_decoder_config decoderConfig = ma_decoder_config_init(AudioDevice::FromAudioFormat(mixingFormat), 0, outputSampleRate);
        //ma_decoding_backend_vtable* pCustomBackendVTables[] = { ma_decoding_backend_libvorbis };
        //decoderConfig.pCustomBackendUserData = nullptr;
        //decoderConfig.ppCustomBackendVTables = pCustomBackendVTables;
        //decoderConfig.customBackendCount = 1;

        ma_decoder decoder;
        if (ma_decoder_init_file(filePath.c_str(), &decoderConfig, &decoder) != MA_SUCCESS)
        {
            delete clip;
            return nullptr;
        }

        //Get Format, Channels, SampleRate, Frames, then Calculate Duration
        ma_format format;
        if (ma_decoder_get_data_format(&decoder, &format, &clip->m_Channels, &clip->m_SampleRate, nullptr, 0) != MA_SUCCESS ||
            ma_decoder_get_length_in_pcm_frames(&decoder, &clip->m_Frames) != MA_SUCCESS)
        {
            delete clip;
            ma_decoder_uninit(&decoder);
            return nullptr;
        }
        clip->m_Format = AudioDevice::ToAudioFormat(format);

        size_t bytesPerFrame = ma_get_bytes_per_frame(format, clip->m_Channels);
        size_t dataSize = static_cast<size_t>(clip->m_Frames) * bytesPerFrame;
        void* pcmData = malloc(dataSize);
        if (pcmData == nullptr)
        {
            delete clip;
            ma_decoder_uninit(&decoder);
            return nullptr;
        }

        uint64_t framesRead = 0;
        ma_result result = ma_decoder_read_pcm_frames(&decoder, pcmData, clip->m_Frames, &framesRead);
        ma_decoder_uninit(&decoder);
        if (result != MA_SUCCESS && result != MA_AT_END)
        {
            delete clip;
            free(pcmData);
            return nullptr;
        }
        clip->m_Frames = framesRead;
        clip->m_Duration = static_cast<double>(clip->m_Frames) / clip->m_SampleRate;

        ma_audio_buffer_config config = ma_audio_buffer_config_init(format, clip->m_Channels, framesRead, pcmData, nullptr);
        result = ma_audio_buffer_alloc_and_init(&config, &clip->m_Buffer);
        free(pcmData);
        if (result != MA_SUCCESS)
        {
            delete clip;
            return nullptr;
        }
    }

    return clip;
}
