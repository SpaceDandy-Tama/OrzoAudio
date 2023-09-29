#include "AudioClip.h"

AudioClip::AudioClip()
{
	m_BufferID = 0;
	m_Format = AudioFormat::None;
	m_Channels = 0;
	m_Frames = 0;
	m_SampleRate = 0;
	m_Duration = 0;
}

AudioClip::~AudioClip()
{
	if (m_BufferID > 0)
		alDeleteBuffers(1, &m_BufferID);
} 

AudioClip* AudioClip::FromFile(const std::string& filePath)
{
	const char* filename = filePath.c_str();
	AudioClip* clip = new AudioClip();
	clip->m_FilePath = filePath;
	ALenum err;
	SNDFILE* sndfile;
	SF_INFO sfinfo;
	short* membuf;
	ALsizei num_bytes;

	/* Open the audio file and check that it's usable. */
	sndfile = sf_open(filename, SFM_READ, &sfinfo);
	if (!sndfile)
	{
		fprintf(stderr, "Count not open audio in %s: %s\n", filename, sf_strerror(sndfile));
		return nullptr;
	}
	if (sfinfo.frames < 1 || sfinfo.frames >(sf_count_t)(INT_MAX / sizeof(short)) / sfinfo.channels)
	{
		fprintf(stderr, "Bad sample count in %s (%" PRId64 ")\n", filename, sfinfo.frames);
		sf_close(sndfile);
		return nullptr;
	}

	/* Get the sound format, and figure out the OpenAL format. */
	clip->m_Format = AudioFormat::None;
	if (sfinfo.channels == 1)
		clip->m_Format = AudioFormat::Mono;
	else if (sfinfo.channels == 2)
		clip->m_Format = AudioFormat::Stereo;
	/*
	else if (sfinfo.channels = 3)
	{
		if (sf_command(sndfile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
			format = AL_FORMAT_BFORMAT2D_16;
	}
	else if (sfinfo.channels = 4)
	{
		if (sf_command(sndfile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
			format = AL_FORMAT_BFORMAT3D_16;
	}
	*/

	if (clip->m_Format == AudioFormat::None)
	{
		fprintf(stderr, "Unsupported channel count: %d\n", sfinfo.channels);
		sf_close(sndfile);
		return nullptr;
	}

	/* Decode the whole audio file to a buffer. */
	membuf = static_cast<short*>(malloc((size_t)(sfinfo.frames * sfinfo.channels) * sizeof(short)));

	clip->m_Frames = sf_readf_short(sndfile, membuf, sfinfo.frames);
	if (clip->m_Frames < 1)
	{
		free(membuf);
		sf_close(sndfile);
		fprintf(stderr, "Failed to read samples in %s (%" PRId64 ")\n", filename, clip->m_Frames);
		return nullptr;
	}
	num_bytes = (ALsizei)(clip->m_Frames * sfinfo.channels) * (ALsizei)sizeof(short);

	clip->m_Channels = sfinfo.channels;
	clip->m_SampleRate = sfinfo.samplerate;
	clip->m_Duration = clip->m_Frames / (double)clip->m_SampleRate;

	/* Buffer the audio data into a new buffer object, then free the data and close the file. */
	clip->m_BufferID = 0;
	alGenBuffers(1, &clip->m_BufferID);
	alBufferData(clip->m_BufferID, clip->m_Format, membuf, num_bytes, sfinfo.samplerate);

	free(membuf);
	sf_close(sndfile);

	/* Check if an error occurred, and clean up if so. */
	err = alGetError();
	if (err != AL_NO_ERROR)
	{
		fprintf(stderr, "OpenAL Error: %s\n", alGetString(err));
		if (clip->m_BufferID && alIsBuffer(clip->m_BufferID))
			alDeleteBuffers(1, &clip->m_BufferID);
		return nullptr;
	}

	return clip;
}
