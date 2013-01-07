#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H

#include "Utility.h"

class SimpleShader;
class SimpleTessShader;
class StandardShader;
class SkeletonMeshShader;
class GradientShader;
class VertexShader;
class PixelShader;
class HullShader;
class DomainShader;
class GeometryShader;
class RenderNormalsShader;
class SkeletonShader;

class ShaderManager
{
private:
	StandardShader*		mStandardShader;
	SkeletonMeshShader* mSkeletonMeshShader;
	GradientShader* mGradientShader;
	RenderNormalsShader* mRenderNormals;
	SkeletonShader*	mSkeletonShader;

	vector<VertexShader*> m_vertexShaders;
	vector<GeometryShader*> m_geometryShaders;
	vector<PixelShader*> m_pixelShaders;
	vector<HullShader*> m_hullShaders;
	vector<DomainShader*> m_domainShaders;

	static ShaderManager* sInstance;
private:
	ShaderManager();
	~ShaderManager();
public:
	static ShaderManager* GetInstance();
	static void Release();

	bool	Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);

	StandardShader* GetStandardShader();
	SkeletonMeshShader* GetSkeletonMeshShader();
	GradientShader* GetGradientShader();
	RenderNormalsShader* GetRenderNormals();
	SkeletonShader* GetSkeletonShader();

};

#endif