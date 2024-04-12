#ifndef GRAPHICSPCH_H
#define GRAPHICSPCH_H

#include <tinyxml2.h>

#include "core/utils/comautoptr.h"
#include "core/file/filesystem.h"
#include "core/file/datastream.h"
#include "core/file/contentmanager.h"

#include "core/object/serializableobject.h"

// we using stuff from engine
#include "engine/enginepch.h"

// graphics stuff
#include "graphics/graphicsapi.h"
#include "graphics/gfxcommon.h"

//# TODO: Make this more beautiful
#ifdef WIN32
#	include <d3d11.h>
#endif // WIN32


#endif // !GRAPHICSPCH_H
