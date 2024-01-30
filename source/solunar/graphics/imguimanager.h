#ifndef IMGUI_MGR_H
#define IMGUI_MGR_H

#include <imgui.h>

namespace engine
{
	class IBufferBase;
	class IShaderProgram;
	class ITexture2D;
	class ISamplerState;
	class IRasterizerState;
	class IDepthStencilState;
	class IBlendState;

	class ImGuiRenderer
	{
	public:
		ImGuiRenderer();
		~ImGuiRenderer();

		void init();
		void shutdown();

		void newFrame();
		void renderDrawData(ImDrawData* drawData);

	private:
		void createFontsTexture();
		void createDeviceObjects();
		void setupRenderState(ImDrawData* draw_data);

	private:
		IBufferBase* m_pVB;
		IBufferBase* m_pIB;
		IBufferBase* m_pVertexConstantBuffer;
		ITexture2D* m_pFontTexture;
		ISamplerState* m_pFontSampler;
		IShaderProgram* m_pShaderProgram;
		IRasterizerState* m_pRasterizerState;
		IDepthStencilState* m_pDepthStencilState;
		IBlendState* m_pBlendState;
		int m_VertexBufferSize;
		int m_IndexBufferSize;
	};

	class ImGuiManager : public Singleton<ImGuiManager>
	{
		static ImGuiManager ms_instance;
	public:
		void init();
		void shutdown();

		void beginFrame();
		void endFrame();

	private:
		void draw();

	private:
		ImGuiRenderer m_ImGuiRenderer;
	};

}

#endif // !IMGUI_MGR_H
