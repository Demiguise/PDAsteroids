#include "AudioPlayer.h"
#include "bass.h"
#include <sys\stat.h>

void __stdcall syncFunc(HSYNC handle, DWORD channel, DWORD data, void* user)
{
	BASS_ChannelRemoveSync(channel, handle);
	BASS_ChannelStop(channel);
}

AudioPlayer::AudioPlayer(HWND hWnd)
{
	if(!BASS_Init(-1, 44100, 0, hWnd, NULL))
	{
		OutputDebugString(L"Can't do shit capt");
	}
	
}

AudioPlayer::~AudioPlayer()
{
	BASS_Free();
}

void AudioPlayer::PlaySoundFile(const char* filename)
{
	BASS_ChannelStop(chan);
	UINT len = strlen(filename);
	size_t convertedChars;
	wchar_t wFileName[100];

	struct stat statbuf;
	stat(filename, &statbuf);

	mbstowcs_s(&convertedChars, wFileName, filename, _TRUNCATE);
	int error = BASS_ErrorGetCode();
	chan = BASS_StreamCreateFile(false, wFileName, 0, 0, BASS_SAMPLE_LOOP);
	error = BASS_ErrorGetCode();

	BASS_ChannelPlay(chan, true);
	error = BASS_ErrorGetCode();
	BASS_ChannelSetSync(chan, BASS_SYNC_END, 0, &syncFunc, 0);
	error = BASS_ErrorGetCode();
}