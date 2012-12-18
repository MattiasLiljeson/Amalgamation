#ifndef SIMPLESHADER_H
#define SIMPLESHADER_H

#include "DX11Application.h"

struct VertexShaderData
{
	ID3D10Blob* CompiledData;
	ID3D11VertexShader* Data;
};

struct PixelShaderData
{
	ID3D10Blob* CompiledData;
	ID3D11PixelShader* Data;
};
struct MatrixBuffer
{
	AglMatrix World;
	AglMatrix View;
	AglMatrix Projection;
};

struct pixelBuffer
{
	float useTexture;
	float buff[3];
};

class SimpleShader
{
private:
	ID3D11Device*			mDevice;
	ID3D11DeviceContext*	mDeviceContext; 
	VertexShaderData		mVSD;
	PixelShaderData			mPSD;
	ID3D11InputLayout*		mIL;
	ID3D11Buffer*			mMatrixBuffer;
	ID3D11Buffer*			mPixelBuffer;
	ID3D11ShaderResourceView* mSrv;
public:
	SimpleShader(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	~SimpleShader();
	VertexShaderData	GetVertexShader();
	PixelShaderData		GetPixelShader();
	ID3D11InputLayout*	GetInputLayout();
	void				SetMatrixBuffer(AglMatrix pWorld, AglMatrix pView, AglMatrix pProjection, bool useTexture);
};

#endif