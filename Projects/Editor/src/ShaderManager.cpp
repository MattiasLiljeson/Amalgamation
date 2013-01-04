#include "ShaderManager.h"
#include "SimpleTessShader.h"
#include "StandardShader.h"
#include "SkeletonMeshShader.h"
#include "GradientShader.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "HullShader.h"
#include "DomainShader.h"
#include "GeometryShader.h"
#include "RenderNormalsShader.h"
#include "SkeletonShader.h"

ShaderManager* ShaderManager::sInstance = NULL;

ShaderManager::ShaderManager()
{
	mStandardShader = NULL;
	mSkeletonMeshShader = NULL;
	mGradientShader = NULL;
	mSkeletonShader = NULL;
	mRenderNormals = NULL;
}
ShaderManager::~ShaderManager()
{
	if (mStandardShader)
		delete mStandardShader;
	if (mSkeletonMeshShader)
		delete mSkeletonMeshShader;
	if (mGradientShader)
		delete mGradientShader;
	if (mSkeletonShader)
		delete mSkeletonShader;
	if (mRenderNormals)
		delete mRenderNormals;

	mStandardShader = NULL;
	mSkeletonMeshShader = NULL;
	mGradientShader = NULL;
	mSkeletonShader = NULL;
	mRenderNormals = NULL;

	for (unsigned int i = 0; i < m_vertexShaders.size(); i++)
		delete m_vertexShaders[i];
	for (unsigned int i = 0; i < m_pixelShaders.size(); i++)
		delete m_pixelShaders[i];
	for (unsigned int i = 0; i < m_hullShaders.size(); i++)
		delete m_hullShaders[i];
	for (unsigned int i = 0; i < m_domainShaders.size(); i++)
		delete m_domainShaders[i];
	for (unsigned int i = 0; i < m_geometryShaders.size(); i++)
		delete m_geometryShaders[i];
}
ShaderManager* ShaderManager::GetInstance()
{
	if (sInstance == NULL)
		sInstance = new ShaderManager();
	return sInstance;
} 
void ShaderManager::Release()
{
	if (sInstance)
		delete sInstance;
	sInstance = NULL;
}
bool ShaderManager::Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	m_vertexShaders.push_back(new VertexShader(pDevice, "Shaders/MeshHLSL.vs", "VShader"));
	m_pixelShaders.push_back(new PixelShader(pDevice, "Shaders/MeshHLSL.ps", "PShader"));
	m_hullShaders.push_back(new HullShader(pDevice, "Shaders/MeshHLSL.hs", "HShader"));
	m_domainShaders.push_back(new DomainShader(pDevice, "Shaders/MeshHLSL.ds", "DShader"));
	mStandardShader = new StandardShader(pDevice, pDeviceContext, m_vertexShaders.back(), m_pixelShaders.back(), m_hullShaders.back(), m_domainShaders.back());

	m_vertexShaders.push_back(new VertexShader(pDevice, "Shaders/skeletonMeshHLSL.vs", "VShader"));
	mSkeletonMeshShader = new SkeletonMeshShader(pDevice, pDeviceContext, m_vertexShaders.back(), m_pixelShaders.back());

	m_vertexShaders.push_back(new VertexShader(pDevice, "Shaders/MeshHLSL.vs", "VShader"));
	m_pixelShaders.push_back(new PixelShader(pDevice, "Shaders/MeshHLSLGradient.ps", "PShader"));
	mGradientShader = new GradientShader(pDevice, pDeviceContext, m_vertexShaders.back(), m_pixelShaders.back());


	//Render normals
	m_vertexShaders.push_back(new VertexShader(pDevice, "Shaders/Skeletonnormals/skeletonMeshHLSLNormal.vs", "VShader"));
	m_vertexShaders.push_back(new VertexShader(pDevice, "Shaders/Skeletonnormals/MeshHLSLNormal.vs", "VShader"));
	m_geometryShaders.push_back(new GeometryShader(pDevice, "Shaders/Skeletonnormals/skeletonMeshHLSLNormal.gs", "GS"));
	m_pixelShaders.push_back(new PixelShader(pDevice, "Shaders/Skeletonnormals/skeletonMeshHLSLNormal.ps", "PShader"));
	mRenderNormals = new RenderNormalsShader(pDevice, pDeviceContext, m_vertexShaders[m_vertexShaders.size()-2], m_vertexShaders.back(), m_geometryShaders.back(), m_pixelShaders.back());

	//Render skeletons
	m_vertexShaders.push_back(new VertexShader(pDevice, "Shaders/Skeleton/skeleton.vs", "VShader"));
	m_pixelShaders.push_back(new PixelShader(pDevice, "Shaders/Skeleton/skeleton.ps", "PShader"));
	mSkeletonShader = new SkeletonShader(pDevice, pDeviceContext, m_vertexShaders.back(), m_pixelShaders.back());


	return true;
}
StandardShader* ShaderManager::GetStandardShader()
{
	return mStandardShader;
}
SkeletonMeshShader* ShaderManager::GetSkeletonMeshShader()
{
	return mSkeletonMeshShader;
}
GradientShader* ShaderManager::GetGradientShader()
{
	return mGradientShader;
}
RenderNormalsShader* ShaderManager::GetRenderNormals()
{
	return mRenderNormals;
}
SkeletonShader* ShaderManager::GetSkeletonShader()
{
	return mSkeletonShader;
}