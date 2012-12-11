#ifndef STANDARDSHADER_H
#define STANDARDSHADER_H

#include "SimpleShader.h"

struct StandardShaderBuffer
{
	AglMatrix World;
	AglMatrix View;
	AglMatrix Projection;
	float  Scale;
	float Pack[3];
};

class StandardShader
{
private:
	ID3D11Device*			mDevice;
	ID3D11DeviceContext*	mDeviceContext; 
	VertexShaderData		mVSD;
	PixelShaderData			mPSD;
	ID3D11InputLayout*		mIL;
	ID3D11Buffer*			mBuffer;
public:
	StandardShader(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	~StandardShader();
	VertexShaderData	GetVertexShader();
	PixelShaderData		GetPixelShader();
	ID3D11InputLayout*	GetInputLayout();
	void				SetBuffer(AglMatrix pWorld, AglMatrix pView, AglMatrix pProjection, float pScale);
};

#endif