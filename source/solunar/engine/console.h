#ifndef ENGINE_CONSOLE_H
#define ENGINE_CONSOLE_H

namespace solunar
{

typedef void (ConsoleCommand)();

class Console;

class ConsoleCommandManager : public Singleton<ConsoleCommandManager>
{
	static ConsoleCommandManager ms_instance;
public:
	void RegisterCommand(const char* name, ConsoleCommand* command);

	void PrintHelp();

	static void Command_Help();

private:
	friend class Console;

	bool Execute(const char* text);

private:
	struct ConsoleCommandReg
	{
		const char* name;
		ConsoleCommand* command;
	};
	std::vector<ConsoleCommandReg> m_commands;
};

class Console
{
public:
	Console();
	~Console();

	void Init();
	void Shutdown();

	bool IsToggled() { return m_isToggled; }
	void ToggleConsole();

	void UpdateConsoleBuffer();

	void OnRender();

	static void Command_ToggleConsole();

private:
	std::vector<char> m_consoleText;
	bool m_isToggled;
};

extern std::unique_ptr<Console> g_console;

}

#endif // !ENGINE_TOOL_CONSOLE_H
