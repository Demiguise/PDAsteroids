#include "GameLog.h"
#include "FileManager.h"

GameLog* GameLog::m_pInstance = 0;

GameLog* GameLog::GetInstance()
{
	if (m_pInstance == NULL)
	{
		m_pInstance = new GameLog;
	}
	return m_pInstance;
}

GameLog::GameLog()
{
	logVerbosity = DebugLevel::Normal;
	writeToLogVerbosity = DebugLevel::High;
	activeChannels = DebugChannel::All;
}

GameLog::~GameLog()
{
	delete m_pInstance;
}

//Always use a _SINGLE_ channel.
void GameLog::Log(const DebugChannel logChannel,  const DebugLevel logLevel, const char* logLine, ...)
{

	if (activeChannels&logChannel) //Bitwise 'AND' operation to check for proper channels
	{
		char buffer[512];
		char* newLogLine = AppendNewlineChar(logLine);
		va_list args;
		va_start(args, logLine);
		vsnprintf_s(buffer, 512, newLogLine, args);
		va_end(args);
		if (logLevel <= logVerbosity)
		{
			OutputDebugStringA((LPCSTR)buffer);
		}
		if (logLevel <= writeToLogVerbosity)
		{
			FileManager::GetInstance()->WriteToLog(buffer);
		}
		delete newLogLine;
	}
}


char* GameLog::AppendNewlineChar(const char* input)
{
	int inputSize = strlen(input);
	char* output = new char[inputSize+2];
	memmove(output, input, inputSize);
	output[inputSize] = '\n';
	output[inputSize+1] = '\0';
	return output;
}