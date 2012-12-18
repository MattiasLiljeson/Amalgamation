#ifndef SIMPLETESSSHADER_H
#define SIMPLETESSSHADER_H

#include "SimpleShader.h"

struct HullShaderData
{
	ID3D10Blob* CompiledData;
	ID3D11HullShader* Data;
};

struct DomainShaderData
{
	ID3D10Blob* CompiledData;
	ID3D11DomainShader* Data;
};

struct GlobalBuffer
{
	AglMatrix World;
	AglMatrix View;
	AglMatrix Projection;
	AglVector3 CameraPosition;
	float Pack[13];
};

class SimpleTessShader
{
private:
	ID3D11Device*			mDevice;
	ID3D11DeviceContext*	mDeviceContext; 
	VertexShaderData		mVSD;
	PixelShaderData			mPSD;
	HullShaderData			mHSD;
	DomainShaderData		mDSD;
	ID3D11InputLayout*		mIL;
	ID3D11Buffer*			mGlobalBuffer;
public:
	SimpleTessShader(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	~SimpleTessShader();
	VertexShaderData	GetVertexShader();
	PixelShaderData		GetPixelShader();
	HullShaderData		GetHullShader();
	DomainShaderData	GetDomainShader();
	ID3D11InputLayout*	GetInputLayout();
	void				SetMatrixBuffer(AglMatrix pWorld, AglMatrix pView, AglMatrix pProjection);
};

#endif