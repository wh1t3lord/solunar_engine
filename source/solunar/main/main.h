#ifndef MAIN_H
#define MAIN_H

namespace engine {
	void appInit(int argc, char** argv);
	void appShutdown();
	void* appGetWindow();
	void appPresent();

	void createEngineView();

	int main(int argc, char* argv[]);

	// commandline class
	class CommandLine
	{
	public:
		void init(int argc, char* argv[]);
		void init(const char* str);

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