#ifndef RMLSYSTEM_H
#define RMLSYSTEM_H

#include "core/utils/singleton.h"
#include "graphics/ui/rmlfilesystem.h"
#include "graphics/ui/rmlrenderer.h"

#if 0

#include <RmlUi/Core.h>
#include <RmlUi/Debugger.h>

namespace engine
{

class RmlSystem : public Singleton<RmlSystem>
{
	static RmlSystem ms_instance;
public:
	RmlSystem();
	~RmlSystem();

	void init();
	void shutdown();

	void createContext(int width, int height);

	void render();

	Rml::Context* getContext();

private:
	// Backend
	RmlFileSystem* m_filesystem;
	RmlRenderer* m_renderer;

	// Context
	Rml::Context* m_context;
};

}

#endif

#endif // !RMLSYSTEM_H
