#pragma once


//0 None (Don't use for logging) | 1 Normal | 2 Errors | 3 Warnings
enum DebugLevel
{
	None = 0, 
	Low,
	Normal,
	High,
	Error,
	Warning
};

//Since it's an int we're using we have 32 possible channels. Sweet!
enum DebugChannel
{
	All = 		-1,
	Main = 		1 << 0,
	Rendering = 	1 << 1,
	Audio = 	1 << 2,
	Input = 	1 << 3,
	Events = 	1 << 4,
	Physics 	1 << 5,
	Unused2 = 	1 << 6,
	Unused3 = 	1 << 7,
};

class GameLog
{
public:
	~GameLog();
	static GameLog* GetInstance();
	void Log(const DebugChannel logChannel, const DebugLevel logLevel, const char* message, ...);
	int logVerbosity;
	int writeToLogVerbosity;
	int activeChannels;
private:
	GameLog();
	static GameLog* m_pInstance;
	char* AppendNewlineChar(const char* input);
};

