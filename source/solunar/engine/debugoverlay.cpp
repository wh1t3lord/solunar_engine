#include "enginepch.h"
#include "engine/debugoverlay.h"
#include "engine/camera.h"

#include "imgui.h"

namespace solunar
{

void solunar::DebugOverlay::render()
{
	static const ImGuiWindowFlags ms_kOverlayWindowFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;

	static int corner = 0;
	ImGuiIO& io = ImGui::GetIO();
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
	if (corner != -1)
	{
		const float PAD = 10.0f;
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImVec2 work_pos = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any!
		ImVec2 work_size = viewport->WorkSize;
		ImVec2 window_pos, window_pos_pivot;
		window_pos.x = (corner & 1) ? (work_pos.x + work_size.x - PAD) : (work_pos.x + PAD);
		window_pos.y = (corner & 2) ? (work_pos.y + work_size.y - PAD) : (work_pos.y + PAD);
		window_pos_pivot.x = (corner & 1) ? 1.0f : 0.0f;
		window_pos_pivot.y = (corner & 2) ? 1.0f : 0.0f;
		ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
		window_flags |= ImGuiWindowFlags_NoMove;
	}

	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

	ImGui::SetNextWindowBgAlpha(0.0f);
	if (ImGui::Begin("Debug overlay", 0, ms_kOverlayWindowFlags))
	{
#if defined(DEBUG) || defined(_DEBUG)
		ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "DEBUG BUILD");
#endif

		ImGui::Text("FPS: %4.1f", io.Framerate);

		Camera* camera = CameraProxy::GetInstance();
		if (camera)
		{
			ImGui::Text("Camera pos: %.2f %.2f %.2f",
				camera->m_position.x,
				camera->m_position.y,
				camera->m_position.z);

			ImGui::Text("Camera dir: %.2f %.2f %.2f",
				camera->m_direction.x,
				camera->m_direction.y,
				camera->m_direction.z);
		}
	}
	ImGui::End();

	ImGui::PopStyleVar();
}

}
