#ifndef STANDARDSHADER_H
#define STANDARDSHADER_H

#include "SimpleShader.h"

struct Material;
class Gradient;
class VertexShader;
class PixelShader;
class DomainShader;
class HullShader;

struct StandardShaderBuffer
{
	AglMatrix World;
	AglMatrix View;
	AglMatrix Projection;
	float  Scale;
	float  TextureScale;
	float Pack[2];
};
struct DomainBuffer
{
	AglMatrix World;
	AglMatrix View;
	AglMatrix Projection;
	float  Scale;
	float Displacement;
	float UseAlpha;
	float Pack;
};
struct HullBuffer
{
	//Edge 1, Edge 2, Edge 3, Inside
	float Amount[4];
};

class StandardShader
{
private:
	ID3D11Device*			mDevice;
	ID3D11DeviceContext*	mDeviceContext; 
	ID3D11InputLayout*		mIL;
	ID3D11Buffer*			mBuffer;
	ID3D11Buffer*			mDomainBuffer;
	ID3D11Buffer*			mMaterialBuffer;
	ID3D11Buffer*			mHullBuffer;

	VertexShader*			mVS;
	PixelShader*			mPS;
	HullShader*				mHS;
	DomainShader*			mDS;

	ID3D11SamplerState*		mSampler;

public:
	StandardShader(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, VertexShader* pVS, PixelShader* pPS, HullShader* pHS, DomainShader* pDS);
	~StandardShader();
	ID3D11VertexShader*	GetVertexShader();
	ID3D11PixelShader*	GetPixelShader();	
	ID3D11HullShader*	GetHullShader();
	ID3D11DomainShader*	GetDomainShader();
	ID3D11InputLayout*	GetInputLayout();
	void				SetBuffer(AglMatrix pWorld, AglMatrix pView, AglMatrix pProjection, float pScale, AglMaterial pMaterial);
	void				SetBuffer(AglMatrix pWorld, AglMatrix pView, AglMatrix pProjection, float pScale, Gradient* pGradient);
};

#endif