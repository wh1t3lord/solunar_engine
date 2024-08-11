#ifndef LOGGER_H
#define LOGGER_H

namespace solunar
{
	class Logger
	{
	public:
		static void Init();
		static void Shutdown();
		static void LogPrint(const char* Msg, ...);
	};

}

#endif // !LOGGER_H
