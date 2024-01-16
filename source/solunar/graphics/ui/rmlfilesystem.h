#ifndef GRAPHICS_UI_RMLFILESYSTEM_H
#define GRAPHICS_UI_RMLFILESYSTEM_H

#if 0
#include <RmlUi/Core/FileInterface.h>

namespace engine
{

class RmlFileSystem : public Rml::FileInterface
{
public:
	Rml::FileHandle Open(const Rml::String& path) override;
	void Close(Rml::FileHandle file) override;

	size_t Read(void* buffer, size_t size, Rml::FileHandle file) override;
	bool Seek(Rml::FileHandle file, long offset, int origin) override;
	size_t Tell(Rml::FileHandle file) override;
/*	size_t Length(Rml::FileHandle file) override; */

	//bool LoadFile(const Rml::String& path, Rml::String& out_data) override;
};

extern RmlFileSystem* g_rmlFileSystem;

}

#endif

#endif // !GRAPHICS_UI_RMLFILESYSTEM_H
