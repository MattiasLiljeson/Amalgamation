#ifndef AGLLOADER_H
#define AGLLOADER_H

#include "Utility.h"
#include "AglReader.h"

class AGLLoader
{
private:
	static AGLLoader* sInstance; 
	char* mPath;
	AglReader* mReader;
	ID3D11Device* mDevice;
	ID3D11DeviceContext* mDeviceContext;
private:
	AGLLoader();
public:
	static AGLLoader* GetInstance();
	static void Release();
	void Init(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	void Load(string pPath);
	~AGLLoader();
};

#endif