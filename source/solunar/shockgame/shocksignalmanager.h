#ifndef SHOCKSIGNALMANAGER_H
#define SHOCKSIGNALMANAGER_H

namespace solunar
{

class ShockSignal : public Object
{
	ImplementObject(ShockSignal, Object);
public:
	ShockSignal();
	~ShockSignal();

	static void registerObject();
};

class ShockSignalManager : public Singleton<ShockSignalManager>
{
////////////////////////////////////////////////
// Singleton
private:
	static ShockSignalManager* ms_instance;

public:
	static void createInstance();
	static void destroyInstance();
	static ShockSignalManager* getInstance();
////////////////////////////////////////////////

public:
	ShockSignalManager();
	~ShockSignalManager();


};

}

#endif // !SHOCKSIGNALMANAGER_H
