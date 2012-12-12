#ifndef SKELETONMESHSHADER_H
#define SKELETONMESHSHADER_H

#include "SimpleShader.h"
#include <AglSkeleton.h>

class Skeleton;
class VertexShader;
class PixelShader;
struct Material;

struct SkeletonMeshShaderBuffer
{
	AglMatrix World;
	AglMatrix View;
	AglMatrix Projection;
	AglMatrix Palette[200];
	float  Scale;
	float  TextureScale;
	float  Pack[2];
};

class SkeletonMeshShader
{
private:
	ID3D11Device*			mDevice;
	ID3D11DeviceContext*	mDeviceContext; 
	ID3D11InputLayout*		mIL;
	ID3D11Buffer*			mBuffer;
	ID3D11Buffer*			mMaterialBuffer;

	VertexShader*			mVS;
	PixelShader*			mPS;

	ID3D11SamplerState*		mSampler;
public:
	SkeletonMeshShader(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, VertexShader* pVS, PixelShader* pPS);
	~SkeletonMeshShader();
	ID3D11VertexShader*	GetVertexShader();
	ID3D11PixelShader*	GetPixelShader();
	ID3D11InputLayout*	GetInputLayout();
	void				SetBuffer(AglMatrix pWorld, AglMatrix pView, AglMatrix pProjection, float pScale, AglSkeleton* pSkeleton, AglMaterial pMaterial);
};

#endif