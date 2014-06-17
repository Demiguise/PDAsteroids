#pragma once
#include "Common.h"
#include <fstream>
#include <sstream>
#include <ctime>
#include <direct.h>

const char logLocation[] = "Game.log";
const char backupLocation[] = "./Logs/";

enum FileType
{
	ObjFile,
	LineFile
};

class FileManager
{
public:
	static FileManager* GetInstance();
	~FileManager();
	ModelData LoadModelData(std::string fileName, FileType fileType);
	void WriteToLog(const char* message);

private:
	FileManager();
	static FileManager* m_pInstance;
	std::istringstream CleanFaceData (std::string line);
	std::vector<std::string> SplitString(std::string line, char delim);
	ModelData ConstructModelData(	std::vector<EnVector3> verts,
									std::vector<EnVector3> normals,
									std::vector<EnVector2> texCoords,
									std::vector<UINT> faces);
	//Loading Functions for different Models
	void Load2DLineFile(std::string fileName, ModelData& model);
	void Load3DObjFile(std::string fileName, ModelData& model);

	std::fstream logStream;
	void GetLocalTime(char* buffer);
	void MoveAndRenameLog();
};

