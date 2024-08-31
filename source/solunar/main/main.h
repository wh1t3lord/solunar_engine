#ifndef MAIN_H
#define MAIN_H

namespace solunar {
	class View;

	void appInit(int argc, char** argv);
	void appShutdown();
	void* appGetWindow();
	View* appGetView();
	void appPresent();
	void appFireExit();

	void CreateEngineView();

	int main(int argc, char* argv[]);

	// commandline class
	class CommandLine
	{
	public:
		void Init(int argc, char* argv[]);
		void Init(const char* str);

		bool hasOption(const char* name);

		const char* getOptionParameter(const char* name);

		std::string getCommandLine();

	private:
		int m_argc;
		char** m_argv;
	};

	extern CommandLine g_commandLine;
}

#endif