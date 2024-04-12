#ifndef SAVELOADMANAGER_H
#define SAVELOADMANAGER_H

// File system
#include "core/file/filesystem.h"

namespace engine {

#define SAVEFILE_MAGIC (('E' << 24) + ('V' << 16) + ('A' << 8) + 'S')
#define SAVEFILE_VERSION 1

struct SaveFileHeader
{
	uint32_t magic;
	uint32_t version;
};

struct SaveFilePlayerInformation
{

};

#define SAVEFILE_MAX_WORLD_NAME_LENGTH 256
struct SaveFileWorldInformation
{
	char worldName[SAVEFILE_MAX_WORLD_NAME_LENGTH];
};

class GAME_API SaveLoadManager
{
public:
	void autosave();
	
private:
	void beginSave(FileHandle file);
	void saveActiveWorld(FileHandle file);
};

extern GAME_API SaveLoadManager g_saveLoadManager;
	
}

#endif