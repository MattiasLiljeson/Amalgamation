#ifndef RENDERNORMALSSHADER
#define RENDERNORMALSSHADER

#include "SimpleShader.h"
#include <AglSkeleton.h>

class Skeleton;
class VertexShader;
class PixelShader;
class GeometryShader;
struct Material;

struct SkeletonMeshShaderNormalBuffer
{
	AglMatrix World;
	AglMatrix View;
	AglMatrix Projection;
	AglMatrix Palette[200];
	float  Scale;
	float Pack[3];
};

struct SkeletonMeshGSShaderNormalBuffer
{
	AglMatrix View;
	AglMatrix Projection;
	float  length[4];
};

struct StandardShaderNormalBuffer
{
	AglMatrix World;
	AglMatrix View;
	AglMatrix Projection;
	float  Scale;
	float Pack[3];
};

class RenderNormalsShader
{
private:
	ID3D11Device*			mDevice;
	ID3D11DeviceContext*	mDeviceContext; 
	ID3D11InputLayout*		mIL;
	ID3D11InputLayout*		mIL2;
	ID3D11Buffer*			mBuffer;
	ID3D11Buffer*			mBuffer2;
	ID3D11Buffer*			mBuffer3;

	VertexShader*			mVS;
	VertexShader*			mVS2;
	GeometryShader*			mGS;
	PixelShader*			mPS;
public:
	RenderNormalsShader(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, VertexShader* pVS, VertexShader* pVS2, GeometryShader* pGS, PixelShader* pPS);
	~RenderNormalsShader();
	ID3D11VertexShader*	GetVertexShader();
	ID3D11VertexShader*	GetVertexShader2();
	ID3D11GeometryShader* GetGeometryShader();
	ID3D11PixelShader*	GetPixelShader();
	ID3D11InputLayout*	GetInputLayout();
	ID3D11InputLayout*	GetInputLayout2();
	void				SetBuffer(AglMatrix pWorld, AglMatrix pView, AglMatrix pProjection, float pScale, AglSkeleton* pSkeleton, float p_length);
	void				SetBuffer2(AglMatrix pWorld, AglMatrix pView, AglMatrix pProjection, float pScale, float p_length);
};

#endif