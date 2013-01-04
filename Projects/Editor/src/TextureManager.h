#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include "Utility.h"
#include <FreeImage.h>

struct TextureData
{
	string ID;
	string Path;
	ID3D11ShaderResourceView* SRV;
};

class TextureManager
{
private:
	static TextureManager* sInstance;

	ID3D11Device* mDevice;
	ID3D11DeviceContext* mDeviceContext;
	vector<TextureData*> mTextureData;
private:
	TextureManager();
	~TextureManager();

	ID3D11ShaderResourceView* loadTexture(ID3D11Device* p_device, 
		const char* p_filePath);
public:
	static TextureManager*	GetInstance();
	static void				Release();

	void					Init(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	int						LoadTexture(string pPath);
	TextureData*			GetTexture(int pIndex);
	TextureData*			GetTexture(string pPath);

	void					ReloadAll();

	static int NormMap;
	static int DispMap;
};

#endif