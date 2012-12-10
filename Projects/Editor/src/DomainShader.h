#ifndef DOMAINSHADER_H
#define DOMAINSHADER_H

#include "Utility.h"

class DomainShader
{
private:
	ID3D11Device*			mDevice;
	ID3D11DomainShader*		mData;
public:
	DomainShader(ID3D11Device* pDevice, string p_path, string p_mainFunction);
	~DomainShader();
	ID3D11DomainShader* GetShader(){ return mData; }
};

#endif