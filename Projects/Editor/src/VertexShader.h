#ifndef VERTEXSHADER_H
#define VERTEXSHADER_H

#include "Utility.h"

class VertexShader
{
private:
	ID3D11Device*			mDevice;
	ID3D11VertexShader*		mData;
	ShaderData*				mShaderData;

	bool					mLoadedFromFile;
public:
	VertexShader(ID3D11Device* pDevice, string p_path, string p_mainFunction);
	~VertexShader();
	ID3D11VertexShader* GetShader(){ return mData; }
	void*			GetCompiledData(){ return mShaderData->data; }
	SIZE_T			GetCompiledSize(){ return mShaderData->size; }
};

#endif