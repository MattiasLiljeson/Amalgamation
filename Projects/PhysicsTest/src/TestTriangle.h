#ifndef TESTTRIANGLE_H
#define TESTTRIANGLE_H

#include "DX11Application.h"
#include "VertexStructures.h"

class TestTriangle
{
private:
	ID3D11Device*			mDevice;
	ID3D11DeviceContext*	mDeviceContext; 
	ID3D11Buffer*			mVB;
	ID3D11Buffer*			mIB;

public:
	TestTriangle(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	~TestTriangle();
	void InitVB();
	void Draw();
};

#endif