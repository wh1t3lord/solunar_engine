#ifndef LOGGER_H
#define LOGGER_H

namespace solunar
{
	class Logger
	{
	public:
		static void init();
		static void shutdown();
		static void logPrint(const char* msg, ...);
	};

}

#endif // !LOGGER_H
