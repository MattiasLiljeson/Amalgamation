#ifndef PIXELSHADER_H
#define PIXELSHADER_H

#include "Utility.h"

class PixelShader
{
private:
	ID3D11Device*			mDevice;
	ID3D11PixelShader*		mData;
public:
	PixelShader(ID3D11Device* pDevice, string p_path, string p_mainFunction);
	~PixelShader();
	ID3D11PixelShader* GetShader(){ return mData; }
};

#endif