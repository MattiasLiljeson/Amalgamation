#ifndef TESTTESSELATEDTRIANGLE_H
#define TESTTESSELATEDTRIANGLE_H

#include "DX11Application.h"
#include "VertexStructures.h"

class TestTesselatedTriangle
{
private:
	ID3D11Device*			mDevice;
	ID3D11DeviceContext*	mDeviceContext; 
	ID3D11Buffer*			mVB;
	ID3D11Buffer*			mIB;

public:
	TestTesselatedTriangle(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	~TestTesselatedTriangle();
	void InitVB();
	void Draw();
};

#endif