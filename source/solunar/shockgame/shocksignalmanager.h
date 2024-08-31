#ifndef SHOCKSIGNALMANAGER_H
#define SHOCKSIGNALMANAGER_H

namespace solunar
{

class ShockSignal : public Object
{
	DECLARE_OBJECT(ShockSignal);
public:
	ShockSignal();
	~ShockSignal();

	static void RegisterObject();
};

class ShockSignalManager : public Singleton<ShockSignalManager>
{
////////////////////////////////////////////////
// Singleton
private:
	static ShockSignalManager* ms_instance;

public:
	static void CreateInstance();
	static void DestroyInstance();
	static ShockSignalManager* GetInstance();
////////////////////////////////////////////////

public:
	ShockSignalManager();
	~ShockSignalManager();


};

}

#endif // !SHOCKSIGNALMANAGER_H
