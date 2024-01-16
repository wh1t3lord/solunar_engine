#ifndef ENGINELOOP_H
#define ENGINELOOP_H

namespace engine {

	class EngineLoop
	{
	public:
		void initialize();
		void shutdown();

		bool update();
	};

}

#endif