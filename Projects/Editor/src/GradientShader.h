#ifndef GRADIENTSHADER_H
#define GRADIENTSHADER_H

#include "SimpleShader.h"
#include <AglGradient.h>

class Gradient;
class VertexShader;
class PixelShader;

struct GradientShaderBuffer
{
	AglMatrix World;
	AglMatrix View;
	AglMatrix Projection;
	float  Scale;
	float Pack[3];
};

class GradientShader
{
private:
	ID3D11Device*			mDevice;
	ID3D11DeviceContext*	mDeviceContext; 
	ID3D11InputLayout*		mIL;
	ID3D11Buffer*			mBuffer;
	ID3D11Buffer*			mGradientBuffer;

	VertexShader*			mVS;
	PixelShader*			mPS;
public:
	GradientShader(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, VertexShader* pVS, PixelShader* pPS);
	~GradientShader();
	ID3D11VertexShader*	GetVertexShader();
	ID3D11PixelShader*  GetPixelShader();
	ID3D11InputLayout*	GetInputLayout();
	void				SetBuffer(AglMatrix pWorld, AglMatrix pView, AglMatrix pProjection, float pScale, AglGradient* pGradient);
};

#endif