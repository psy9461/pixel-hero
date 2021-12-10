#include "iOpenAL.h"

#include "iStd.h"

ALvoid* gStaticBufferData = NULL;

iOpenAL::iOpenAL(int audioNum)
{
	bufCount = audioNum;
	gBuffer = new ALuint[audioNum];
	gSource = new ALuint[audioNum];

	ALCdevice* newDevice = NULL;
	ALCcontext* newContext = NULL;
	ALenum error;

	newDevice = alcOpenDevice(NULL);
	if (newDevice)
	{
		const ALCint attrList[] = { ALC_FREQUENCY, 22050, 0 };
		newContext = alcCreateContext(newDevice, attrList);
		if (newContext)
		{
			alcMakeContextCurrent(newContext);

			alGenBuffers(bufCount, gBuffer);
			if ((error = alGetError()) != AL_NO_ERROR)
			{
				printf("Error alGenBuffers %d\n", error);
				return;
			}

			alGenSources(bufCount, gSource);
			if ((error = alGetError()) != AL_NO_ERROR)
			{
				printf("Error alGenSources %d\n", error);
				return;
			}
		}
	}

	// clean any errors
#ifdef _DEBUG
	printf("Clean any errors %d\n", alGetError());
#else
	alGetError();
#endif // _DEBUG

}

iOpenAL::~iOpenAL()
{
	//alDeleteSources(bufCount, gBuffer);
	//alDeleteBuffers(bufCount, gSource);
	ALCuint* returnNames = new ALCuint[bufCount];
	alDeleteSources(bufCount, returnNames);
	alDeleteBuffers(bufCount, returnNames);

	ALCcontext* context = alcGetCurrentContext();
	ALCdevice* device = alcGetContextsDevice(context);
	alcDestroyContext(context);
	alcCloseDevice(device);
	if (gStaticBufferData)
		delete gStaticBufferData;
}

typedef struct _WaveHeader0 {
	char riff[4];				// Chunk ID: "RIFF"
	unsigned int riffSize;		// Chunk size: 4+n
	char wave[4];				// WAVE ID: "WAVE"

	char fmt[4];				// Chunk ID: "fmt"
	unsigned int fmtSize;		// Chunk size: 16 or 18 or 40
	unsigned short format;		// Format code
	unsigned short channels;	// Number of interleaved channels
	unsigned int samplesPerSec;	// Sampling rate (blocks per second)
	unsigned int bytesPerSec;	// Data rate
	unsigned short blockAlign;	// Data block size(bytes)
	unsigned short bitsPerSample;// Bits per sample
} WaveHeader0;

typedef struct _WaveHeader1 {
	char data[4];				//'data'
	unsigned int dataSize;
} WaveHeader1;

void iOpenAL::initBuffer(int idx, const char* szFormat, ...)
{
	char szText[256];
	va_start_end(szText, szFormat);

	int bufLen;
	char* buf = loadFile(szText, bufLen);

	WaveHeader0* head0 = (WaveHeader0*)buf;
	//if (memcmp("RIFF", head0->riff, 4) ||
	//	memcmp("WAVE", head0->wave, 4) ||
	//	memcmp("fmt", head0->fmt, 4))// strcmp("fmt", head0->fmt)
	//{
	//	printf("Not wave file!!\n");
	//	return;
	//}
	WaveHeader1* head1 = (WaveHeader1*)&buf[sizeof(WaveHeader0) + (head0->fmtSize - 16)];

	ALuint buffer = 0;
	ALuint format = 0;
	switch (head0->bitsPerSample) {
	case 8:	format = (head0->channels == 1 ? AL_FORMAT_MONO8 : AL_FORMAT_STEREO8); break;
	case 16:format = (head0->channels == 1 ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16); break;
	default: return;
	}

	ALsizei samplesPerSec = head0->samplesPerSec;

	char* data = &buf[sizeof(WaveHeader0) + (head0->fmtSize - 16) + sizeof(WaveHeader1)];
	int dataLength = head1->dataSize;

	alBufferData(gBuffer[idx], format, data, dataLength, samplesPerSec);

	delete buf;
	ALenum error;
	if ((error = alGetError()) != AL_NO_ERROR)
		printf("Error attaching audio to buffer: [%s] %x\n", szText, error);
}

void iOpenAL::initSource(int idx, bool repeat)
{
	ALenum error = AL_NO_ERROR;
	alGetError();// Clear any error

	alSourcei(gSource[idx], AL_LOOPING, repeat);

	const float sourcePosAL[] = { 0.0f, 0.0f, 0.0f };// x, y, distance
	alSourcefv(gSource[idx], AL_POSITION, sourcePosAL);

	alSourcef(gSource[idx], AL_REFERENCE_DISTANCE, 50.0f);

	alSourcei(gSource[idx], AL_BUFFER, gBuffer[idx]);

	if ((error = alGetError()) != AL_NO_ERROR)
		printf("Error attaching buffer to source %x\n", error);
}

void iOpenAL::play(int idx, bool repeat)
{
	alSourcePlay(gSource[idx]);

	ALenum error;
	if ((error = alGetError()) != AL_NO_ERROR)
	{
		printf("Error starting source: %x\n", error);
	}
}

void iOpenAL::pause(int idx)
{
	alSourcePause(gSource[idx]);

	ALenum error;
	if ((error = alGetError()) != AL_NO_ERROR)
	{
		printf("Error Pause source: %x\n", error);
	}
}

void iOpenAL::stop(int idx)
{
	alSourceStop(gSource[idx]);

	ALenum error;
	if ((error = alGetError()) != AL_NO_ERROR)
	{
		printf("Error Stop source: %x\n", error);
	}
}

void iOpenAL::volume(int idx, float vol)
{
	alSourcef(gSource[idx], AL_GAIN, vol);
}

static iOpenAL* al;
AudioInfo* audioInfo;
static int audioNum;
static int prevBgm;

void loadAudio(AudioInfo* ai, int aiNum)
{
	al = new iOpenAL(aiNum);

	audioInfo = new AudioInfo[aiNum];
	for (int i = 0; i < aiNum; i++)
	{
		AudioInfo* a0 = &ai[i];
		AudioInfo* a1 = &audioInfo[i];
		al->initBuffer(i, a0->fileName);
		al->initSource(i, a0->repeat);
		a1->repeat = a0->repeat;
		a1->volume = a0->volume;
	}
	audioNum = aiNum;

	prevBgm = -1;
}

void freeAudio()
{
	delete al;
	delete audioInfo;
}

void pauseAudio()
{
	for (int i = 0; i < audioNum; i++)
	{
		if (audioInfo[i].repeat)
			al->pause(i);
	}
}

void resumeAudio()
{
	if (prevBgm != -1)
		audioPlay(prevBgm);
}

void audioPlay(int index)
{
	if (index > audioNum - 1)
		return;

	al->play(index, false);

	if (audioInfo[index].repeat)
		prevBgm = index;
}

void audioStop(int index)
{
	al->stop(index);
}

void audioStop()
{
	for (int i = 0; i < audioNum; i++)
	{
		if (audioInfo[i].repeat)
			al->stop(i);
	}
	prevBgm = -1;
}

void audioVolume(float bgm, float eff, int effNum)
{
	if (bgm < 0.1) bgm = 0;	else if (bgm > 0.9) bgm = 1;
	if (eff < 0.1) eff = 0;	else if (eff > 0.9) eff = 1;

	int i;
	for (i = 0; i < effNum; i++)
		al->volume(i, eff * audioInfo[i].volume);
	for (i = effNum; i < audioNum; i++)
		al->volume(i, bgm * audioInfo[i].volume);
}
