#include "enginepch.h"
#include "engine/console.h"

#include "engine/inputmanager.h"

#include "imgui.h"

namespace solunar
{

std::unique_ptr<Console> g_console = std::make_unique<Console>();

Console::Console() :
	m_isToggled(false)
{
}

Console::~Console()
{
}

void Console::Init()
{
	ConsoleCommandManager::GetInstance()->RegisterCommand("toggleconsole", &Console::Cmd_ToggleConsole);
}

void Console::Shutdown()
{
}

void Console::ToggleConsole()
{
	if (m_isToggled)
		m_consoleText.clear();

	m_isToggled = !m_isToggled;
}

void Console::UpdateConsoleBuffer()
{
	InputManager* input = InputManager::GetInstance();

	if (m_isToggled)
	{
		if (input->IsPressedWithReset(KeyboardKeys::KEY_BACKSPACE) && !m_consoleText.empty())
			m_consoleText.pop_back();

		if (input->IsPressedWithReset(KeyboardKeys::KEY_ENTER))
		{
			std::vector<char> localConsoleText = m_consoleText;
			localConsoleText.push_back('\0');

			Core::Msg("> %s", localConsoleText.data());
			ConsoleCommandManager::GetInstance()->Execute(localConsoleText.data());

			ToggleConsole();
		}
		else
		{
			char lastChar = input->GetLastCharacter();
			if (lastChar < 32 || lastChar > 126)
				return;

			m_consoleText.push_back(lastChar);
		}
	}
}

void Console::OnRender()
{
	//m_consoleText.push_back(InputManager::getInstance()->getLastCharacter());

	UpdateConsoleBuffer();

#if 0

	const float PAD = 10.0f;
	const float PAD_INNER = 12.0f;
	const float FAT = 125.0f;
	const float FAT_INNER = 20.0f;

	ImGuiIO& io = ImGui::GetIO();
	ImDrawList* drawList = ImGui::GetForegroundDrawList();

	drawList->AddRectFilled(ImVec2(PAD, FAT- PAD_INNER), ImVec2(io.DisplaySize.x - PAD, FAT), 0xff0000ff);
#endif

#if 1
	static const ImGuiWindowFlags ms_kOverlayWindowFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;

	static int corner = 2;
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
	if (ImGui::Begin("Console", 0, ms_kOverlayWindowFlags))
	{

		ImGui::Text(">");

		ImGui::SameLine();

		std::vector<char> localConsoleText = m_consoleText;
		localConsoleText.push_back('\0');

		ImGui::Text("%s", localConsoleText.data());
			
	}
	ImGui::End();

	ImGui::PopStyleVar();
#endif
}

void Console::Cmd_ToggleConsole()
{
	g_console->ToggleConsole();
}

ConsoleCommandManager ConsoleCommandManager::ms_instance;

void ConsoleCommandManager::RegisterCommand(const char* name, ConsoleCommand* command)
{
	Assert2(name, "Command name cannot be null.");
	Assert2(command, "Command cannot be null.");

	ConsoleCommandReg reg;
	reg.name = name;
	reg.command = command;
	m_commands.push_back(reg);
}

void ConsoleCommandManager::Execute(const char* text)
{
	int numCommands = (int)m_commands.size();

	for (int i = 0; i < numCommands; i++)
	{
		const ConsoleCommandReg& reg = m_commands[i];
		Assert2(reg.command, "Internal error. Console commands cannot be null!");
		if (strcmp(reg.name, text) == 0)
			reg.command();
	}
}


}