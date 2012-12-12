#ifndef GEOMETRYSHADER_H
#define GEOMETRYSHADER_H

#include "Utility.h"

class GeometryShader
{
private:
	ID3D11Device*			mDevice;
	ID3D11GeometryShader*		mData;
public:
	GeometryShader(ID3D11Device* pDevice, string p_path, string p_mainFunction);
	~GeometryShader();
	ID3D11GeometryShader* GetShader(){ return mData; }
};

#endif