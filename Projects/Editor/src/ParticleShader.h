// =======================================================================================
//                                      ParticleShader
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Brief
///        
/// # ParticleShader
/// Detailed description.....
/// Created on: 4-1-2013 
///---------------------------------------------------------------------------------------
#ifndef PARTICLESHADER_H
#define PARTICLESHADER_H

#include "SimpleShader.h"
#include <AglGradient.h>
#include "ParticleSystem.h"

class Gradient;
class VertexShader;
class PixelShader;
class GeometryShader;

struct ParticleBuffer
{
	AglVector4 EyePosition;
	AglMatrix View;
	AglMatrix Projection;
	AglVector4 Color;
	float FadeIn;
	float FadeOut;
	float ParticleMaxAge;
	float Opacity;
	AglVector4 CameraZ;
	AglVector4 CameraY;
	int Alignment;
	float buff[3];
};

class ParticleShader
{
private:
	ID3D11Device*			mDevice;
	ID3D11DeviceContext*	mDeviceContext; 
	ID3D11InputLayout*		mIL;
	ID3D11Buffer*			mBuffer;

	VertexShader*			mVS;
	PixelShader*			mPS;
	GeometryShader*			mGS;
public:
	ParticleShader(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, VertexShader* pVS, PixelShader* pPS, GeometryShader* pGS);
	~ParticleShader();
	ID3D11VertexShader*	GetVertexShader();
	ID3D11PixelShader*  GetPixelShader();
	ID3D11GeometryShader* GetGeometryShader();
	ID3D11InputLayout*	GetInputLayout();
	void SetBuffer(ParticleSystem* pPS);
};

#endif // PARTICLESHADER_H