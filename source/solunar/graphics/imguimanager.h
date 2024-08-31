#ifndef IMGUI_MGR_H
#define IMGUI_MGR_H

#include <imgui.h>

namespace solunar
{
	struct IBufferBase;
	class IShaderProgram;
	class ITexture2D;
	class ISamplerState;
	struct IRasterizerState;
	struct IDepthStencilState;
	struct IBlendState;

	class ImGuiRenderer
	{
	public:
		ImGuiRenderer();
		~ImGuiRenderer();

		void Init();
		void Shutdown();

		void newFrame();
		void renderDrawData(ImDrawData* drawData);

	private:
		void createFontsTexture();
		void createDeviceObjects();
		void invalidateDeviceObjects();
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
		void Init();
		void Shutdown();

		void BeginFrame();
		void EndFrame();

	private:
		void Draw();

	private:
		ImGuiRenderer m_ImGuiRenderer;
	};

}

#endif // !IMGUI_MGR_H
