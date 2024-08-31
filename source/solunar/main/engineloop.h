#ifndef ENGINELOOP_H
#define ENGINELOOP_H

namespace solunar {

	class EngineLoop
	{
	public:
		void Initialize();
		void Shutdown();

		bool Update();
	};

}

#endif