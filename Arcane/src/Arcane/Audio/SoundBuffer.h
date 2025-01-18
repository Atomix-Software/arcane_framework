#pragma once

#include "Arcane/Core/Base.h"

#include <AL/al.h>
#include <sndfile.h>

namespace Arcane
{
	class ARC_API SoundLibrary
	{

	public:
		SoundLibrary();
		~SoundLibrary();

		ALuint Load(const std::string& filename);
		bool Unload(const std::string& filename);
		bool Unload(const ALuint& buffer);

	public:
		static Shared<SoundLibrary> Create();

	private:
		std::unordered_map<std::string, ALuint> m_SoundBuffers;

	};

	class ARC_API MusicBuffer
	{
	public:
		static const int BUFFER_SAMPLES = 8192;
		static const int NUM_BUFFERS = 1024;

	public:
		MusicBuffer(const std::string& filename);
		~MusicBuffer();

		void Play();
		void Stop();
		void Pause();
		void Resume();

		void UpdateBufferStream();
		void SetLooping(const bool& loop);

		ALint GetSourceId();
		bool IsPlaying();

	private:
		ALuint m_SourceId;
		ALuint m_SoundBuffers[NUM_BUFFERS];

		SNDFILE* m_SoundFile;
		SF_INFO m_SFinfo;
		short* m_MemBuffer;
		ALenum m_Format;
	};
}