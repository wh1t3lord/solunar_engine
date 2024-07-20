#ifndef D3D11DRV_PCH_H
#define D3D11DRV_PCH_H

#include <unordered_map>

// Direct3D 12
#include <d3d12.h>
//#include <dxgi1_4.h>
#include <dxgi1_6.h>
//#include <d3dx12.h>

// we using stuff from engine
#include "engine/enginepch.h"

// graphics stuff
#include "graphics/gfxcommon.h"

// d3d12 utils
#include "d3d12drv/d3d12utils.h"

// Device
#include "d3d12drv/d3d12device.h"

#endif // !D3D11DRV_PCH_H
