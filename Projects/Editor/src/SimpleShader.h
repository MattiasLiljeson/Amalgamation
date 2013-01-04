#ifndef SIMPLESHADER_H
#define SIMPLESHADER_H

#include "DX11Application.h"

struct MaterialBuffer
{
	AglVector4 AmbientOpacity;
	AglVector4 DiffuseReflectivity;
	AglVector4 SpecularShininess;
	AglVector4 EmissiveDiffuseMapped;
	AglVector4 EyePositionSpecularMapped;
	AglVector4 Flags;
};
struct GradientBuffer
{
	AglVector4 AmbientOpacity[10];
	AglVector4 DiffuseReflectivity[10];
	AglVector4 Flags;
};

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

class SimpleShader
{
private:
	ID3D11Device*			mDevice;
	ID3D11DeviceContext*	mDeviceContext; 
	VertexShaderData		mVSD;
	PixelShaderData			mPSD;
	ID3D11InputLayout*		mIL;
	ID3D11Buffer*			mMatrixBuffer;
public:
	SimpleShader(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	~SimpleShader();
	VertexShaderData	GetVertexShader();
	PixelShaderData		GetPixelShader();
	ID3D11InputLayout*	GetInputLayout();
	void				SetMatrixBuffer(AglMatrix pWorld, AglMatrix pView, AglMatrix pProjection);
};

#endif