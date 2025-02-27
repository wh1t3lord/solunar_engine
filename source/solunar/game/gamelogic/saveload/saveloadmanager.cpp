#include "gamepch.h"
#include "game/gamelogic/saveload/saveloadmanager.h"



namespace solunar {
	
SaveLoadManager g_saveLoadManager;
	
void SaveLoadManager::autosave()
{
#if 0
	char buffer[260];
	snprintf(buffer, 260, "autosave.sav");
	
	File* file = FileDevice::GetInstance()->openFile(buffer, FileAccess::Write);
	
	// write header
	beginSave(file);
	
	// write world entities
	saveActiveWorld(file);
	
	FileDevice::GetInstance()->closeFile(file);
#endif
}

void SaveLoadManager::beginSave(FileHandle file)
{
#if 0
	SaveFileHeader h;
	h.magic = SAVEFILE_MAGIC;
	h.version = SAVEFILE_VERSION;
	file->Write(&h, sizeof(h));
#endif
}

void SaveLoadManager::saveActiveWorld(FileHandle file)
{
#if 0
	SaveFileWorldInformation worldInfo;
	strcpy(worldInfo.worldName, "Invalid World Name");
	
	file->Write(&worldInfo, sizeof(worldInfo));

	std::shared_ptr<World> world = WorldManager::getActiveWorld();

	tinyxml2::XMLDocument doc;
	tinyxml2::XMLElement* worldElement = doc.NewElement("World");
	doc.InsertFirstChild(worldElement);
	
	world->SaveXML(*worldElement);

	tinyxml2::XMLPrinter printer(0, true, 0);
	doc.Accept(&printer);

	size_t stringLength = strlen(printer.CStr());
	file->Write(&stringLength, sizeof(stringLength));
	
	file->Write((char*)printer.CStr(), stringLength);
#endif
}

}