#ifndef MESH_H
#define MESH_H

#include "Utility.h"
#include "AglReader.h"
#include <AglGradient.h>
#include <AglBoundingSphere.h>
#include <AglOBB.h>

class SkeletonMapping;
class Scene;
struct Material;
class Gradient;

class Mesh
{
private:
	//DirectX related
	ID3D11Device*			mDevice;
	ID3D11DeviceContext*	mDeviceContext;
	ID3D11Buffer*			mVB;
	ID3D11Buffer*			mIB;
	int						mStride;

	//Bounding box
	AglVector3					mMin;
	AglVector3					mMax;

	vector<SkeletonMapping*> mSkeletonMappings;

	Scene*		mScene;
	AglMesh*	mMesh;

	bool	mVisible;
	int		mCurrentMaterial;

	vector<AglGradient*> mGradients;

	bool m_wireframe;
	float m_normalLength;

	AglLooseBspTree* bsptree;
	AglInteriorSphereGrid* m_grid;
	bool m_drawTree;
	bool m_drawSphereGrid;
	unsigned int m_treeLevel;


	//Temp
	AglVector3 spos;

	AglMaterial* overrideMaterial;

public:
	Mesh(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, Scene* pScene);
	~Mesh();

	void Init(AglMesh* pMesh);
	template <class T>
	void Init(T* pVertices, int pVertexCount, unsigned int* pIndices, int pIndexCount);

	AglVector3 GetMin();
	AglVector3 GetMax();

	void Draw(AglMatrix pWorld, float pScale = 1.0f);
	void DrawNormals(AglMatrix pWorld, float pScale);

	void AddSkeletonMapping(SkeletonMapping* pSkeletonMapping);
	void SetMaterial(int pMaterial);
	void AddGradient(AglGradient* pGradient, bool pSetAsCurrent);

	int					  GetIndexCount();
	int					  GetVertexCount();
	void				  SetVisible(bool pVisible){ mVisible = pVisible; }
	bool				  IsVisible(){ return mVisible; }
	string				  GetName();
	void				  SetName(string pName);
	void				  SetWireframe(bool p_wireframe){ m_wireframe = p_wireframe; }
	bool				  GetWireframe(){ return m_wireframe; }
	void				  SetDrawNormals(float p_drawNormals){ m_normalLength = p_drawNormals; }
	float				  GetDrawNormals(){ return m_normalLength; }

	void				  SetBspTree(AglLooseBspTree* p_tree){ bsptree = p_tree; m_treeLevel = 0;}
	void				  SetInteriorSpheres(AglInteriorSphereGrid* p_grid){ m_grid = p_grid;}	

	void update(float p_elapsedTime);

	AglBoundingSphere	getBoundingSphere();
	AglOBB				getMinimumOBB();

	void				createSphereGrid();
	void				createBspTree();

	bool*				getDrawTree(){ return &m_drawTree; }
	bool*				getDrawGrid(){ return &m_drawSphereGrid; }
	unsigned int*		getTreeLevel(){ return &m_treeLevel; }
	AglMatrix			getTransform(){ return mMesh->getHeader().transform; }
	bool				hasSkeleton(){ return mSkeletonMappings.size() > 0;}
	void				setOverrideMaterial(AglMaterial* p_mat){ overrideMaterial = p_mat;}
	int					getCurrentMaterial(){ return mCurrentMaterial; }
};

template <typename T>
void Mesh::Init(T* pVertices, int pVertexCount, unsigned int* pIndices, int pIndexCount)
{
	//Create Vertex Buffer
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(T) * pVertexCount;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = pVertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	mDevice->CreateBuffer(&bd, &vertexData, &mVB);

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.ByteWidth = sizeof(unsigned int) * pIndexCount;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = pIndices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	mDevice->CreateBuffer(&ibd, &indexData, &mIB);

	mStride = sizeof(T);
}
#endif