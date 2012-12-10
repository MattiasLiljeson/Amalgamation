#ifndef HULLSHADER_H
#define HULLSHADER_H

#include "Utility.h"

class HullShader
{
private:
	ID3D11Device*			mDevice;
	ID3D11HullShader*		mData;
public:
	HullShader(ID3D11Device* pDevice, string p_path, string p_mainFunction);
	~HullShader();
	ID3D11HullShader* GetShader(){ return mData; }
};

#endif