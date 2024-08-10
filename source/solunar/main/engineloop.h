#ifndef ENGINELOOP_H
#define ENGINELOOP_H

namespace solunar {

	class EngineLoop
	{
	public:
		void initialize();
		void shutdown();

		bool update();
	};

}

#endif