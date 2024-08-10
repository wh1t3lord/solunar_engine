#include "graphicspch.h"
#include "graphics/imguimanager.h"

// Graphics Core
#include "graphics/core/buffer.h"
#include "graphics/core/rendertarget.h"
#include "graphics/core/texture.h"
#include "graphics/core/statemanager.h"
#include "graphics/core/device.h"

// Shader Manager
#include "graphics/shaderprogram.h"
#include "graphics/ShaderProgramManager.h"

#ifdef _WIN32
#include <imgui_impl_win32.h>
#include "main/main.h"
#endif // _WIN32

namespace solunar
{
	struct VERTEX_CONSTANT_BUFFER_DX11
	{
		float   mvp[4][4];
	};

	struct ImGuiBackendData
	{

	};

	ImGuiManager ImGuiManager::ms_instance;

	ImGuiRenderer::ImGuiRenderer() :
		m_pVB(nullptr),
		m_pIB(nullptr),
		m_pVertexConstantBuffer(nullptr),
		m_pFontTexture(nullptr),
		m_pFontSampler(nullptr),
		m_pShaderProgram(nullptr),
		m_pRasterizerState(nullptr),
		m_pBlendState(nullptr),
		m_pDepthStencilState(nullptr),
		m_VertexBufferSize(0),
		m_IndexBufferSize(0)
	{
		m_VertexBufferSize = 5000;
		m_IndexBufferSize = 10000;
	}

	ImGuiRenderer::~ImGuiRenderer()
	{
	}

	void ImGuiRenderer::init()
	{
		ImGuiIO& io = ImGui::GetIO();
		IM_ASSERT(io.BackendRendererUserData == nullptr && "Already initialized a renderer backend!");

		// Setup backend capabilities flags
		ImGuiBackendData* bd = IM_NEW(ImGuiBackendData)();
		io.BackendRendererUserData = (void*)bd;
		io.BackendRendererName = "imgui_impl_solunar";
		io.BackendFlags |= ImGuiBackendFlags_RendererHasVtxOffset;  // We can honor the ImDrawCmd::VtxOffset field, allowing for large meshes.
	}

	void ImGuiRenderer::shutdown()
	{
		invalidateDeviceObjects();

		ImGuiIO& io = ImGui::GetIO();
		ImGuiBackendData* bd = (ImGuiBackendData*)io.BackendRendererUserData;

		io.BackendRendererName = nullptr;
		io.BackendRendererUserData = nullptr;
		io.BackendFlags &= ~ImGuiBackendFlags_RendererHasVtxOffset;

		IM_DELETE(bd);
	}

	void ImGuiRenderer::newFrame()
	{
		if (!m_pFontSampler)
			createDeviceObjects();
	}

	void ImGuiRenderer::renderDrawData(ImDrawData* draw_data)
	{    
		// Avoid rendering when minimized
		if (draw_data->DisplaySize.x <= 0.0f || draw_data->DisplaySize.y <= 0.0f)
			return;

		// Create and grow vertex/index buffers if needed
		if (!m_pVB || m_VertexBufferSize < draw_data->TotalVtxCount)
		{
			if (m_pVB) { mem_delete(m_pVB); m_pVB = nullptr; }
			m_VertexBufferSize = draw_data->TotalVtxCount + 5000;

			BufferDesc desc = {};
			desc.m_bufferMemorySize = m_VertexBufferSize * sizeof(ImDrawVert);
			desc.m_bufferAccess = BufferAccess::Dynamic;
			desc.m_bufferType = BufferType::VertexBuffer;

			SubresourceDesc subResource = {};
			
			m_pVB = g_renderDevice->createBuffer(desc, subResource);
		}
		
		if (!m_pIB || m_IndexBufferSize < draw_data->TotalIdxCount)
		{
			if (m_pIB) { mem_delete(m_pIB); m_pIB = nullptr; }
			
			m_IndexBufferSize = draw_data->TotalIdxCount + 10000;
			
			BufferDesc desc = {};
			desc.m_bufferMemorySize = m_IndexBufferSize * sizeof(ImDrawIdx);
			desc.m_bufferAccess = BufferAccess::Dynamic;
			desc.m_bufferType = BufferType::IndexBuffer;

			SubresourceDesc subResource = {};
			
			m_pIB = g_renderDevice->createBuffer(desc, subResource);
		}

		// Upload vertex/index data into a single contiguous GPU buffer
		ImDrawVert* vtx_dst = (ImDrawVert*)m_pVB->map(BufferMapping::WriteOnly);
		ImDrawIdx* idx_dst = (ImDrawIdx*)m_pIB->map(BufferMapping::WriteOnly);
		for (int n = 0; n < draw_data->CmdListsCount; n++)
		{
			const ImDrawList* cmd_list = draw_data->CmdLists[n];
			memcpy(vtx_dst, cmd_list->VtxBuffer.Data, cmd_list->VtxBuffer.Size * sizeof(ImDrawVert));
			memcpy(idx_dst, cmd_list->IdxBuffer.Data, cmd_list->IdxBuffer.Size * sizeof(ImDrawIdx));
			vtx_dst += cmd_list->VtxBuffer.Size;
			idx_dst += cmd_list->IdxBuffer.Size;
		}
		m_pIB->unmap();
		m_pVB->unmap();
		
		// Setup orthographic projection matrix into our constant buffer
		// Our visible imgui space lies from draw_data->DisplayPos (top left) to draw_data->DisplayPos+data_data->DisplaySize (bottom right). DisplayPos is (0,0) for single viewport apps.
		{
#if 1
			VERTEX_CONSTANT_BUFFER_DX11* constant_buffer = (VERTEX_CONSTANT_BUFFER_DX11*)m_pVertexConstantBuffer->map(BufferMapping::WriteOnly);
			float L = draw_data->DisplayPos.x;
			float R = draw_data->DisplayPos.x + draw_data->DisplaySize.x;
			float T = draw_data->DisplayPos.y;
			float B = draw_data->DisplayPos.y + draw_data->DisplaySize.y;
			float mvp[4][4] =
			{
				{ 2.0f/(R-L),   0.0f,           0.0f,       0.0f },
				{ 0.0f,         2.0f/(T-B),     0.0f,       0.0f },
				{ 0.0f,         0.0f,           0.5f,       0.0f },
				{ (R+L)/(L-R),  (T+B)/(B-T),    0.5f,       1.0f },
			};
			memcpy(&constant_buffer->mvp, mvp, sizeof(mvp));
			m_pVertexConstantBuffer->unmap();
#else
			// calculate ortho matrix based on current view
			glm::mat4 proj = glm::ortho(0.0f, draw_data->DisplaySize.x, 0.0f, draw_data->DisplaySize.y, 0.1f, 100.0f);

			VERTEX_CONSTANT_BUFFER_DX11* constant_buffer = (VERTEX_CONSTANT_BUFFER_DX11*)m_pVertexConstantBuffer->map(BufferMapping::WriteOnly);
			memcpy(&constant_buffer->mvp, &proj[0], sizeof(proj));
			m_pVertexConstantBuffer->unmap();
#endif
		}

		// Setup desired DX state
		setupRenderState(draw_data);

		// Render command lists
		// (Because we merged all buffers into a single one, we maintain our own offset into them)
		int global_idx_offset = 0;
		int global_vtx_offset = 0;
		ImVec2 clip_off = draw_data->DisplayPos;
		for (int n = 0; n < draw_data->CmdListsCount; n++)
		{
			const ImDrawList* cmd_list = draw_data->CmdLists[n];
			for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++)
			{
				const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
				if (pcmd->UserCallback != nullptr)
				{
					// User callback, registered via ImDrawList::AddCallback()
					// (ImDrawCallback_ResetRenderState is a special callback value used by the user to request the renderer to reset render state.)
					if (pcmd->UserCallback == ImDrawCallback_ResetRenderState)
						setupRenderState(draw_data);
					else
						pcmd->UserCallback(cmd_list, pcmd);
				}
				else
				{
					// Project scissor/clipping rectangles into framebuffer space
					ImVec2 clip_min(pcmd->ClipRect.x - clip_off.x, pcmd->ClipRect.y - clip_off.y);
					ImVec2 clip_max(pcmd->ClipRect.z - clip_off.x, pcmd->ClipRect.w - clip_off.y);
					if (clip_max.x <= clip_min.x || clip_max.y <= clip_min.y)
						continue;

					// Apply scissor/clipping rectangle
					g_renderDevice->setScissors(clip_min.x, clip_min.y, clip_max.x, clip_max.y);

					// Bind texture, Draw
					ITexture2D* texture = (ITexture2D*)pcmd->GetTexID();
					g_renderDevice->setTexture2D(0, texture);
					g_renderDevice->setSampler(0, m_pFontSampler); // #TODO: THIS IS SHIT
					g_renderDevice->drawIndexed(PM_TriangleList,  pcmd->IdxOffset + global_idx_offset, pcmd->ElemCount, pcmd->VtxOffset + global_vtx_offset);

				}
			}
			global_idx_offset += cmd_list->IdxBuffer.Size;
			global_vtx_offset += cmd_list->VtxBuffer.Size;
		}
	}

	void ImGuiRenderer::createFontsTexture()
	{
		// Build texture atlas
		ImGuiIO& io = ImGui::GetIO();

		unsigned char* pixels = nullptr;
		int width = 0, height = 0;
		io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);
	
		// Upload texture to graphics system
		{
			TextureDesc desc = {};
			desc.m_textureType = TextureType::Texture2D;
			desc.m_width = width;
			desc.m_height = height;
			desc.m_mipmapLevel = 1;
			desc.m_format = ImageFormat::RGBA32;

			SubresourceDesc subResource = {};
			subResource.m_memory = pixels;
			subResource.m_memoryPitch = desc.m_width * 4;
			subResource.m_memorySlicePitch = 0;

			m_pFontTexture = g_renderDevice->createTexture2D(desc, subResource);
		}

		// Store our identifier
		io.Fonts->SetTexID((ImTextureID)m_pFontTexture);

		// Create texture sampler
		// (Bilinear sampling is required by default. Set 'io.Fonts->Flags |= ImFontAtlasFlags_NoBakedLines' or 'style.AntiAliasedLinesUseTex = false' to allow point/nearest sampling)
		{
			SamplerDesc desc = {};
			desc.m_minFilter = TextureFilter::LinearMipmapLinear;
			desc.m_magFilter = TextureFilter::Linear;
			desc.m_wrapS = TextureWrap::Repeat;
			desc.m_wrapT = TextureWrap::Repeat;
			desc.m_wrapRepeat = TextureWrap::Repeat;
			desc.m_anisotropyLevel = 1.0f;

			m_pFontSampler = g_stateManager->createSamplerState(desc);
		}
	}

	void ImGuiRenderer::createDeviceObjects()
	{
		if (m_pFontSampler)
			invalidateDeviceObjects();

		// Create the input layout
		InputLayoutDesc local_layout[] =
		{
			{ "POSITION", 0, ImageFormat::RG32F,   0, (UINT)offsetof(ImDrawVert, pos), INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, ImageFormat::RG32F,   0, (UINT)offsetof(ImDrawVert, uv),  INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR",    0, ImageFormat::RGBA32_UNORM, 0, (UINT)offsetof(ImDrawVert, col), INPUT_PER_VERTEX_DATA, 0 },
		};

		// Create the shader program
		m_pShaderProgram = g_shaderManager->createShaderProgram("imgui_backend.vsh", "imgui_backend.psh", nullptr,
			local_layout, sizeof(local_layout) / sizeof(local_layout[0]));
	
		// Create the constant buffer
		{
			BufferDesc desc = {};
			desc.m_bufferMemorySize = sizeof(VERTEX_CONSTANT_BUFFER_DX11);
			desc.m_bufferAccess = BufferAccess::Dynamic;
			desc.m_bufferType = BufferType::ConstantBuffer;

			SubresourceDesc subResource = {};
			m_pVertexConstantBuffer = g_renderDevice->createBuffer(desc, subResource);
		}

		// Create the blending setup
		{
			BlendStateDesc desc						= {};
			desc.m_alphaToCoverageEnable			= false;
			desc.m_renderTarget[0].m_blendEnable	= true;
			desc.m_renderTarget[0].m_srcBlend		= BLEND_SRC_ALPHA;
			desc.m_renderTarget[0].m_destBlend		= BLEND_INV_SRC_ALPHA;
			desc.m_renderTarget[0].m_blendOp		= BLEND_OP_ADD;
			desc.m_renderTarget[0].m_srcBlendAlpha	= BLEND_ONE;
			desc.m_renderTarget[0].m_destBlendAlpha = BLEND_INV_SRC_ALPHA;
			desc.m_renderTarget[0].m_blendOpAlpha	= BLEND_OP_ADD;
			desc.m_renderTarget[0].m_renderTargetWriteMask = COLOR_WRITE_ENABLE_ALL;
			m_pBlendState = g_stateManager->createBlendState(desc);
		}

		// Create the rasterizer state
		{
			RasterizerStateDesc desc = {};
			desc.m_fillMode = FillMode::Solid;
			desc.m_cullMode = CullMode::None;
			desc.m_scissorEnable = true;
			desc.m_depthClipEnable = true;
			desc.m_frontCCW = false;
			m_pRasterizerState = g_stateManager->createRasterizerState(desc);
		}

		// Create depth-stencil State
		{
			DepthStencilDesc desc = {};
			desc.m_depthEnable = false;
			desc.m_depthWriteMask = DEPTH_WRITE_MASK_ALL;
			desc.m_depthFunc = COMPARISON_ALWAYS;
			desc.m_stencilEnable = false;
			desc.m_frontFace.m_stencilFailOp = desc.m_frontFace.m_stencilDepthFailOp = desc.m_frontFace.m_stencilPassOp = STENCIL_OP_KEEP;
			desc.m_frontFace.m_stencilFunc = COMPARISON_ALWAYS;
			desc.m_backFace = desc.m_frontFace;
			m_pDepthStencilState = g_stateManager->createDepthStencilState(desc);
		}

		createFontsTexture();
	}

	void ImGuiRenderer::invalidateDeviceObjects()
	{
		// States are cached and not should be deleted
		//if (bd->pFontSampler) { bd->pFontSampler->Release(); bd->pFontSampler = nullptr; }
		//if (bd->pBlendState) { bd->pBlendState->Release(); bd->pBlendState = nullptr; }
		//if (bd->pDepthStencilState) { bd->pDepthStencilState->Release(); bd->pDepthStencilState = nullptr; }
		//if (bd->pRasterizerState) { bd->pRasterizerState->Release(); bd->pRasterizerState = nullptr; }

		// Shaders are cached too
		//if (bd->pPixelShader) { bd->pPixelShader->Release(); bd->pPixelShader = nullptr; }
		//if (bd->pVertexShader) { bd->pVertexShader->Release(); bd->pVertexShader = nullptr; }
	
		//if (bd->pInputLayout) { bd->pInputLayout->Release(); bd->pInputLayout = nullptr; }

		if (m_pFontTexture) { mem_delete(m_pFontTexture); m_pFontTexture = nullptr; ImGui::GetIO().Fonts->SetTexID(0); } // We copied data->pFontTextureView to io.Fonts->TexID so let's clear that as well.
		if (m_pIB) { mem_delete(m_pIB); m_pIB = nullptr; }
		if (m_pVB) { mem_delete(m_pVB); m_pVB = nullptr; }
		if (m_pVertexConstantBuffer) { mem_delete(m_pVertexConstantBuffer); m_pVertexConstantBuffer = nullptr; }
	}

	void ImGuiRenderer::setupRenderState(ImDrawData* draw_data)
	{   
		// Setup viewport
		Viewport vp;
		memset(&vp, 0, sizeof(Viewport));
		vp.m_width = (int)draw_data->DisplaySize.x;
		vp.m_height = (int)draw_data->DisplaySize.y;
		vp.m_x = vp.m_y = 0;
		g_renderDevice->setViewport(&vp);

		// Setup shader and vertex buffers
		unsigned int stride = sizeof(ImDrawVert);
		unsigned int offset = 0;
		g_renderDevice->setVertexBuffer(m_pVB, stride, offset);
		g_renderDevice->setIndexBuffer(m_pIB, sizeof(ImDrawIdx) == 2 ? true : false);
		g_renderDevice->setConstantBufferIndex(0, m_pVertexConstantBuffer);
		g_shaderManager->setShaderProgram(m_pShaderProgram);

		// Setup blend state
		const float blend_factor[4] = { 0.f, 0.f, 0.f, 0.f };
		g_stateManager->setBlendState(m_pBlendState, blend_factor, 0xffffffff);
		g_stateManager->setDepthStencilState(m_pDepthStencilState, 0);
		g_stateManager->setRasterizerState(m_pRasterizerState);
	}

	void ImGuiStyleCustom()
	{
		ImGuiStyle& style = ImGui::GetStyle();
		style.WindowRounding = 5.3f;
		style.FrameRounding = 2.3f;
		style.ScrollbarRounding = 0;

		style.Colors[ImGuiCol_Text] = ImVec4(0.90f, 0.90f, 0.90f, 0.90f);
		style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
		style.Colors[ImGuiCol_WindowBg] = ImVec4(0.09f, 0.09f, 0.15f, 1.00f);
		//style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		style.Colors[ImGuiCol_PopupBg] = ImVec4(0.05f, 0.05f, 0.10f, 0.85f);
		style.Colors[ImGuiCol_Border] = ImVec4(0.70f, 0.70f, 0.70f, 0.65f);
		style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		style.Colors[ImGuiCol_FrameBg] = ImVec4(0.00f, 0.00f, 0.01f, 1.00f);
		style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.90f, 0.80f, 0.80f, 0.40f);
		style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.90f, 0.65f, 0.65f, 0.45f);
		style.Colors[ImGuiCol_TitleBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.83f);
		style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.40f, 0.40f, 0.80f, 0.20f);
		style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.00f, 0.00f, 0.00f, 0.87f);
		style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.01f, 0.01f, 0.02f, 0.80f);
		style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.20f, 0.25f, 0.30f, 0.60f);
		style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.55f, 0.53f, 0.55f, 0.51f);
		style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.56f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.91f);
	//	style.Colors[ImGuiCol_ComboBg] = ImVec4(0.1f, 0.1f, 0.1f, 0.99f);
		style.Colors[ImGuiCol_CheckMark] = ImVec4(0.90f, 0.90f, 0.90f, 0.83f);
		style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.70f, 0.70f, 0.70f, 0.62f);
		style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.30f, 0.30f, 0.30f, 0.84f);
		style.Colors[ImGuiCol_Button] = ImVec4(0.48f, 0.72f, 0.89f, 0.49f);
		style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.50f, 0.69f, 0.99f, 0.68f);
		style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.80f, 0.50f, 0.50f, 1.00f);
		style.Colors[ImGuiCol_Header] = ImVec4(0.30f, 0.69f, 1.00f, 0.53f);
		style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.44f, 0.61f, 0.86f, 1.00f);
		style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.38f, 0.62f, 0.83f, 1.00f);
//		style.Colors[ImGuiCol_Column] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
//		style.Colors[ImGuiCol_ColumnHovered] = ImVec4(0.70f, 0.60f, 0.60f, 1.00f);
//		style.Colors[ImGuiCol_ColumnActive] = ImVec4(0.90f, 0.70f, 0.70f, 1.00f);
		style.Colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 1.00f, 1.00f, 0.85f);
		style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.60f);
		style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(1.00f, 1.00f, 1.00f, 0.90f);
	//	style.Colors[ImGuiCol_CloseButton] = ImVec4(0.50f, 0.50f, 0.90f, 0.50f);
	//	style.Colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.70f, 0.70f, 0.90f, 0.60f);
	//	style.Colors[ImGuiCol_CloseButtonActive] = ImVec4(0.70f, 0.70f, 0.70f, 1.00f);
		style.Colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
		style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
		style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
		style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.00f, 0.00f, 1.00f, 0.35f);
	//	style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);
	}

	void ImGuiCustomStyle2()
	{
		ImGuiStyle& style = ImGui::GetStyle();

		// light style from Pac�me Danhiez (user itamago) https://github.com/ocornut/imgui/pull/511#issuecomment-175719267
		style.Alpha = 1.0f;
		style.FrameRounding = 3.0f;
		style.Colors[ImGuiCol_Text] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
		style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
		style.Colors[ImGuiCol_WindowBg] = ImVec4(0.94f, 0.94f, 0.94f, 0.94f);
//		style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		style.Colors[ImGuiCol_PopupBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.94f);
		style.Colors[ImGuiCol_Border] = ImVec4(0.00f, 0.00f, 0.00f, 0.39f);
		style.Colors[ImGuiCol_BorderShadow] = ImVec4(1.00f, 1.00f, 1.00f, 0.10f);
		style.Colors[ImGuiCol_FrameBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.94f);
		style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
		style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
		style.Colors[ImGuiCol_TitleBg] = ImVec4(0.96f, 0.96f, 0.96f, 1.00f);
		style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 1.00f, 1.00f, 0.51f);
		style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.82f, 0.82f, 0.82f, 1.00f);
		style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.98f, 0.98f, 0.98f, 0.53f);
		style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.69f, 0.69f, 0.69f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.59f, 0.59f, 0.59f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.49f, 0.49f, 0.49f, 1.00f);
//		style.Colors[ImGuiCol_ComboBg] = ImVec4(0.86f, 0.86f, 0.86f, 0.99f);
		style.Colors[ImGuiCol_CheckMark] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.24f, 0.52f, 0.88f, 1.00f);
		style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		style.Colors[ImGuiCol_Button] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
		style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.06f, 0.53f, 0.98f, 1.00f);
		style.Colors[ImGuiCol_Header] = ImVec4(0.26f, 0.59f, 0.98f, 0.31f);
		style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
		style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
//		style.Colors[ImGuiCol_Column] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
//		style.Colors[ImGuiCol_ColumnHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.78f);
//		style.Colors[ImGuiCol_ColumnActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		style.Colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 1.00f, 1.00f, 0.50f);
		style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
		style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
//		style.Colors[ImGuiCol_CloseButton] = ImVec4(0.59f, 0.59f, 0.59f, 0.50f);
//		style.Colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.98f, 0.39f, 0.36f, 1.00f);
//		style.Colors[ImGuiCol_CloseButtonActive] = ImVec4(0.98f, 0.39f, 0.36f, 1.00f);
		style.Colors[ImGuiCol_PlotLines] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
		style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
		style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
		style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
		style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
	//	style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);

		float alpha_ = 1.0f;
		bool bStyleDark_ = true;

		if (bStyleDark_)
		{
			for (int i = 0; i <= ImGuiCol_COUNT; i++)
			{
				ImVec4& col = style.Colors[i];
				float H, S, V;
				ImGui::ColorConvertRGBtoHSV(col.x, col.y, col.z, H, S, V);

				if (S < 0.1f)
				{
					V = 1.0f - V;
				}
				ImGui::ColorConvertHSVtoRGB(H, S, V, col.x, col.y, col.z);
				if (col.w < 1.00f)
				{
					col.w *= alpha_;
				}
			}
		}
		else
		{
			for (int i = 0; i <= ImGuiCol_COUNT; i++)
			{
				ImVec4& col = style.Colors[i];
				if (col.w < 1.00f)
				{
					col.x *= alpha_;
					col.y *= alpha_;
					col.z *= alpha_;
					col.w *= alpha_;
				}
			}
		}
	}
	
	void imguiInitGoldSrcStyle()
	{
		ImVec4* colors = ImGui::GetStyle().Colors;
		colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
		colors[ImGuiCol_WindowBg] = ImVec4(0.29f, 0.34f, 0.26f, 1.00f);
		colors[ImGuiCol_ChildBg] = ImVec4(0.29f, 0.34f, 0.26f, 1.00f);
		colors[ImGuiCol_PopupBg] = ImVec4(0.24f, 0.27f, 0.20f, 1.00f);
		colors[ImGuiCol_Border] = ImVec4(0.54f, 0.57f, 0.51f, 0.50f);
		colors[ImGuiCol_BorderShadow] = ImVec4(0.14f, 0.16f, 0.11f, 0.52f);
		colors[ImGuiCol_FrameBg] = ImVec4(0.24f, 0.27f, 0.20f, 1.00f);
		colors[ImGuiCol_FrameBgHovered] = ImVec4(0.27f, 0.30f, 0.23f, 1.00f);
		colors[ImGuiCol_FrameBgActive] = ImVec4(0.30f, 0.34f, 0.26f, 1.00f);
		colors[ImGuiCol_TitleBg] = ImVec4(0.24f, 0.27f, 0.20f, 1.00f);
		colors[ImGuiCol_TitleBgActive] = ImVec4(0.29f, 0.34f, 0.26f, 1.00f);
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
		colors[ImGuiCol_MenuBarBg] = ImVec4(0.24f, 0.27f, 0.20f, 1.00f);
		colors[ImGuiCol_ScrollbarBg] = ImVec4(0.35f, 0.42f, 0.31f, 1.00f);
		colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.28f, 0.32f, 0.24f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.25f, 0.30f, 0.22f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.23f, 0.27f, 0.21f, 1.00f);
		colors[ImGuiCol_CheckMark] = ImVec4(0.59f, 0.54f, 0.18f, 1.00f);
		colors[ImGuiCol_SliderGrab] = ImVec4(0.35f, 0.42f, 0.31f, 1.00f);
		colors[ImGuiCol_SliderGrabActive] = ImVec4(0.54f, 0.57f, 0.51f, 0.50f);
		colors[ImGuiCol_Button] = ImVec4(0.29f, 0.34f, 0.26f, 0.40f);
		colors[ImGuiCol_ButtonHovered] = ImVec4(0.35f, 0.42f, 0.31f, 1.00f);
		colors[ImGuiCol_ButtonActive] = ImVec4(0.54f, 0.57f, 0.51f, 0.50f);
		colors[ImGuiCol_Header] = ImVec4(0.35f, 0.42f, 0.31f, 1.00f);
		colors[ImGuiCol_HeaderHovered] = ImVec4(0.35f, 0.42f, 0.31f, 0.6f);
		colors[ImGuiCol_HeaderActive] = ImVec4(0.54f, 0.57f, 0.51f, 0.50f);
		colors[ImGuiCol_Separator] = ImVec4(0.14f, 0.16f, 0.11f, 1.00f);
		colors[ImGuiCol_SeparatorHovered] = ImVec4(0.54f, 0.57f, 0.51f, 1.00f);
		colors[ImGuiCol_SeparatorActive] = ImVec4(0.59f, 0.54f, 0.18f, 1.00f);
		colors[ImGuiCol_ResizeGrip] = ImVec4(0.19f, 0.23f, 0.18f, 0.00f); // grip invis
		colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.54f, 0.57f, 0.51f, 1.00f);
		colors[ImGuiCol_ResizeGripActive] = ImVec4(0.59f, 0.54f, 0.18f, 1.00f);
		colors[ImGuiCol_Tab] = ImVec4(0.35f, 0.42f, 0.31f, 1.00f);
		colors[ImGuiCol_TabHovered] = ImVec4(0.54f, 0.57f, 0.51f, 0.78f);
		colors[ImGuiCol_TabActive] = ImVec4(0.59f, 0.54f, 0.18f, 1.00f);
		colors[ImGuiCol_TabUnfocused] = ImVec4(0.24f, 0.27f, 0.20f, 1.00f);
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.35f, 0.42f, 0.31f, 1.00f);
		//colors[ImGuiCol_DockingPreview] = ImVec4(0.59f, 0.54f, 0.18f, 1.00f);
		//colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
		colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
		colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.59f, 0.54f, 0.18f, 1.00f);
		colors[ImGuiCol_PlotHistogram] = ImVec4(1.00f, 0.78f, 0.28f, 1.00f);
		colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
		colors[ImGuiCol_TextSelectedBg] = ImVec4(0.59f, 0.54f, 0.18f, 1.00f);
		colors[ImGuiCol_DragDropTarget] = ImVec4(0.73f, 0.67f, 0.24f, 1.00f);
		colors[ImGuiCol_NavHighlight] = ImVec4(0.59f, 0.54f, 0.18f, 1.00f);
		colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
		colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
		colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);

		ImGuiStyle& style = ImGui::GetStyle();
		style.FrameBorderSize = 1.0f;
		style.WindowRounding = 0.0f;
		style.ChildRounding = 0.0f;
		style.FrameRounding = 0.0f;
		style.PopupRounding = 0.0f;
		style.ScrollbarRounding = 0.0f;
		style.GrabRounding = 0.0f;
		style.TabRounding = 0.0f;

		ImGuiIO& io = ImGui::GetIO();
		io.Fonts->AddFontFromFileTTF("data/textures/ui/system.ttf", 16.0f);
	}

	void ImGuiManager::init()
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		(void)io;

		ImGui::StyleColorsDark();
		//ImGuiStyleCustom();
		//ImGuiCustomStyle2();
		//imguiInitGoldSrcStyle();

		// Initialize backend
#ifdef _WIN32
		ImGui_ImplWin32_Init(appGetWindow());
#endif // _WIN32

		m_ImGuiRenderer.init();
	}

	void ImGuiManager::shutdown()
	{
		m_ImGuiRenderer.shutdown();

#ifdef _WIN32
		ImGui_ImplWin32_Shutdown();
#endif // _WIN32

		ImGui::DestroyContext();
	}

	void ImGuiManager::beginFrame()
	{
#ifdef _WIN32
		ImGui_ImplWin32_NewFrame();
#endif // _WIN32

		m_ImGuiRenderer.newFrame();

		ImGui::NewFrame();
	}

	void ImGuiManager::endFrame()
	{
		draw();
	}

	void ImGuiManager::draw()
	{
		ImGui::Render();
		m_ImGuiRenderer.renderDrawData(ImGui::GetDrawData());
	}
}
