#ifndef SKELETONSHADER_H
#define SKELETONSHADER_H

#include "Utility.h"
#include "SkeletonMesh.h"
#include "PixelShader.h"
#include "VertexShader.h"

struct SkeletonShaderBuffer
{
	AglMatrix World;
	AglMatrix View;
	AglMatrix Projection;
	AglMatrix Palette[200];
	float  Scale;
	float  Pack[3];
};

class SkeletonShader
{
private:
	ID3D11Device*			mDevice;
	ID3D11DeviceContext*	mDeviceContext; 
	ID3D11InputLayout*		mIL;
	ID3D11Buffer*			mBuffer;

	VertexShader*			mVS;
	PixelShader*			mPS;

public:
	SkeletonShader(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, VertexShader* pVS, PixelShader* pPS);
	~SkeletonShader();
	ID3D11VertexShader*	GetVertexShader();
	ID3D11PixelShader*	GetPixelShader();
	ID3D11InputLayout*	GetInputLayout();
	void				SetBuffer(AglMatrix pWorld, AglMatrix pView, AglMatrix pProjection, float pScale, SkeletonMesh* pSkeletonMesh);
};

#endif