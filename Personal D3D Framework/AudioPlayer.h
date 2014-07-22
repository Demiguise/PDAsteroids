#pragma once
#include <Windows.h>

#define NUM_BUFFERS 3
#define BUFFER_SIZE 4096

class AudioPlayer
{
public:
	AudioPlayer(HWND hWnd);
	~AudioPlayer();
	void PlaySoundFile(const char* filename);

private:
	unsigned long chan;
};

