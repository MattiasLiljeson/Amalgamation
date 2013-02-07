#include "BufferFactory.h"
#include "Mesh.h"
#include "PVertex.h"
#include <AglSphereMesh.h>
#include "LightMesh.h"
#include "ParticleCBuffer.h"
#include <LightInstanceData.h>

BufferFactory::BufferFactory(ID3D11Device* p_device, ID3D11DeviceContext* p_deviceContext)
{
	m_device = p_device;
	m_deviceContext = p_deviceContext;
}

BufferFactory::~BufferFactory()
{

}

Buffer<SimpleCBuffer>*  BufferFactory::createSimpleCBuffer()
{
	Buffer<SimpleCBuffer>* cBuffer;
	/// initialization data
	SimpleCBuffer data={
		{1.0f,1.0f,1.0f,  1.0f},// color
		{1.0f,0.0f,0.0f,  0.0f, // this here is an identity matrix
		 0.0f,1.0f,0.0f,  0.0f,
		 0.0f,0.0f,1.0f,  0.0f,
		 0.0f,0.0f,0.0f,  1.0f}
	};
	UINT32 elemSz = sizeof(float)*4; // applicable even if we have matrices, 
									 // hlsl only cares about total size of buffer

	// set up buffer description: usage, type and size
	BufferConfig::BUFFER_INIT_DESC bufferDesc;
	bufferDesc.ElementSize = elemSz;
	bufferDesc.Usage = BufferConfig::BUFFER_CPU_WRITE_DISCARD;
	bufferDesc.NumElements = sizeof(data)/elemSz;
	bufferDesc.Type = BufferConfig::CONSTANT_BUFFER_VS_PS;
	bufferDesc.Slot = BufferConfig::PEROBJECT;
	

	// create and return the buffer
	cBuffer = new Buffer<SimpleCBuffer>(m_device,m_deviceContext,&data,bufferDesc);
	return cBuffer;
}

Buffer<PTVertex>* BufferFactory::createFullScreenQuadBuffer()
{
	PTVertex mesh[]= {
		{{ 1,	-1,	0},	{ 1, 1}},
		{{ -1,	-1,	0},	{ 0, 1}},
		{{ 1,	1,	0},	{ 1, 0}},

		{{ -1, -1,	0},	{ 0, 1}},
		{{ 1,	1,	0},	{ 1, 0}},
		{{ -1,	1,	0},	{ 0, 0}}
	};
	Buffer<PTVertex>* quadBuffer;

	// Create description for buffer
	BufferConfig::BUFFER_INIT_DESC bufferDesc;
	bufferDesc.ElementSize = sizeof(PTVertex);
	bufferDesc.Usage = BufferConfig::BUFFER_DEFAULT;
	bufferDesc.NumElements = 6;
	bufferDesc.Type = BufferConfig::VERTEX_BUFFER;
	bufferDesc.Slot = BufferConfig::SLOT0;

	// Create buffer from config and data
	quadBuffer = new Buffer<PTVertex>(m_device,m_deviceContext,&mesh[0],bufferDesc);

	return quadBuffer;
}

Buffer<InstanceData>* BufferFactory::createInstanceBuffer( InstanceData* p_instanceList, 
														  unsigned int p_numberOfElements)
{
	Buffer<InstanceData>* instanceBuffer;

	// Create description for buffer
	BufferConfig::BUFFER_INIT_DESC bufferDesc;
	bufferDesc.ElementSize = sizeof(InstanceData);
	bufferDesc.Usage = BufferConfig::BUFFER_DEFAULT;
	bufferDesc.NumElements = p_numberOfElements;
	bufferDesc.Type = BufferConfig::VERTEX_BUFFER;
	bufferDesc.Slot = BufferConfig::SLOT0;

	// Create buffer from config and data
	instanceBuffer = new Buffer<InstanceData>(m_device,m_deviceContext,
											    p_instanceList,bufferDesc);

	return instanceBuffer;
}

Buffer<LightInstanceData>* BufferFactory::createLightInstanceBuffer( 
	LightInstanceData* p_instanceList, unsigned int p_numberOfElements )
{
	Buffer<LightInstanceData>* instanceBuffer;

	// Create description for buffer
	BufferConfig::BUFFER_INIT_DESC bufferDesc;
	bufferDesc.ElementSize = sizeof( LightInstanceData );
	bufferDesc.Usage = BufferConfig::BUFFER_DEFAULT;
	bufferDesc.NumElements = p_numberOfElements;
	bufferDesc.Type = BufferConfig::VERTEX_BUFFER;
	bufferDesc.Slot = BufferConfig::SLOT0;

	// Create buffer from config and data
	instanceBuffer = new Buffer<LightInstanceData>( m_device, m_deviceContext,
		p_instanceList, bufferDesc );

	return instanceBuffer;
}


Buffer<DIndex>* BufferFactory::createIndexBuffer( DIndex* p_indices, 
												 unsigned int p_numberOfElements )
{	
	Buffer<DIndex>* indexBuffer;

	// Create description for buffer
	BufferConfig::BUFFER_INIT_DESC indexBufferDesc;
	indexBufferDesc.ElementSize = sizeof(DIndex);
	indexBufferDesc.Usage = BufferConfig::BUFFER_DEFAULT;
	indexBufferDesc.NumElements = p_numberOfElements;
	indexBufferDesc.Type = BufferConfig::INDEX_BUFFER;
	indexBufferDesc.Slot = BufferConfig::SLOT0;

	indexBuffer = new Buffer<DIndex>(m_device,m_deviceContext, p_indices,
									 indexBufferDesc);

	return indexBuffer;
}

Buffer<ParticleCBuffer>* BufferFactory::createParticleCBuffer(){
	ParticleCBuffer data;

	UINT32 elemSz = sizeof(float)*4;

	BufferConfig::BUFFER_INIT_DESC bD;
	bD.ElementSize = elemSz;
	bD.Usage = BufferConfig::BUFFER_CPU_WRITE_DISCARD;
	bD.NumElements = sizeof(data)/elemSz;
	bD.Type = BufferConfig::CONSTANT_BUFFER_VS_GS_PS;
	bD.Slot = BufferConfig::PEROBJECT;

	return new Buffer<ParticleCBuffer>(m_device,m_deviceContext,&data,bD);
}

Buffer<RenderSceneInfoCBuffer>* BufferFactory::createRenderSceneInfoCBuffer(){
	RenderSceneInfoCBuffer data;

	UINT32 elementSize = sizeof(float)*4; // 16 byte alignment

	BufferConfig::BUFFER_INIT_DESC bufferDesc;
	bufferDesc.ElementSize = elementSize;
	bufferDesc.Usage = BufferConfig::BUFFER_CPU_WRITE_DISCARD;
	bufferDesc.NumElements = sizeof(data)/elementSize;
	bufferDesc.Type = BufferConfig::CONSTANT_BUFFER_ALL;
	bufferDesc.Slot = BufferConfig::PERFRAME;

	return new Buffer<RenderSceneInfoCBuffer>(m_device,m_deviceContext,&data,bufferDesc);
}

Buffer<ShadowCBuffer>* BufferFactory::createShadowBuffer(){

	ShadowCBuffer data;
	UINT32 elementSize = sizeof(float)*4; // 16 byte alignment

	BufferConfig::BUFFER_INIT_DESC bufferDesc;
	bufferDesc.ElementSize = elementSize;
	bufferDesc.Usage = BufferConfig::BUFFER_CPU_WRITE_DISCARD;
	bufferDesc.NumElements = sizeof(data)/elementSize;
	bufferDesc.Type = BufferConfig::CONSTANT_BUFFER_ALL;
	bufferDesc.Slot = BufferConfig::SHADOWBUFFER;

	return new Buffer<ShadowCBuffer>(m_device,m_deviceContext,&data,bufferDesc);
}

Buffer<PerShadowCBuffer>* BufferFactory::createPerShadowBuffer()
{
	PerShadowCBuffer data;
	UINT32 elementSize = sizeof(float)*4;

	BufferConfig::BUFFER_INIT_DESC bufferDesc;
	bufferDesc.ElementSize = elementSize;
	bufferDesc.Usage = BufferConfig::BUFFER_CPU_WRITE_DISCARD;
	bufferDesc.NumElements = sizeof(data)/elementSize;
	bufferDesc.Type = BufferConfig::CONSTANT_BUFFER_VS;
	bufferDesc.Slot = BufferConfig::PEROBJECT;

	return new Buffer<PerShadowCBuffer>( m_device, m_deviceContext, &data, bufferDesc);
}


Mesh* BufferFactory::createBoxMesh()
{
#pragma region static data
	PNTTBVertex mesh[]= {
		{	{-1,-1,-1},	{0,0,-1},	{0,1}, {0,0,0}, {0,0,0}	},
		{	{-1,1,-1},	{0,0,-1},	{0,0}, {0,0,0}, {0,0,0}	},
		{	{1,1,-1},	{0,0,-1},	{1,0}, {0,0,0}, {0,0,0}	},
		{	{1,-1,-1},	{0,0,-1},	{1,1}, {0,0,0}, {0,0,0}	},
										
		{	{-1,-1,1},	{0,0,1}	,	{1,1}, {0,0,0}, {0,0,0}	},
		{	{1,-1,1},	{0,0,1}	,	{0,1}, {0,0,0}, {0,0,0}	},
		{	{1,1,1},	{0,0,1}	,	{0,0}, {0,0,0}, {0,0,0}	},
		{	{-1,1,1},	{0,0,1}	,	{1,0}, {0,0,0}, {0,0,0}	},
										
		{	{-1,1,-1},	{0,1,0}	,	{0,1}, {0,0,0}, {0,0,0}	},
		{	{-1,1,1},	{0,1,0}	,	{0,0}, {0,0,0}, {0,0,0}	},
		{	{1,1,1},	{0,1,0}	,	{1,0}, {0,0,0}, {0,0,0}	},
		{	{1,1,-1},	{0,1,0}	,	{1,1}, {0,0,0}, {0,0,0}	},
										
		{	{-1,-1,-1},	{0,-1,0},	{1,1}, {0,0,0}, {0,0,0}	},
		{	{1,-1,-1},	{0,-1,0},	{0,1}, {0,0,0}, {0,0,0}	},
		{	{1,-1,1},	{0,-1,0},	{0,0}, {0,0,0}, {0,0,0}	},
		{	{-1,-1,1},	{0,-1,0},	{1,0}, {0,0,0}, {0,0,0}	},
										 
		{	{-1,-1,1},	{-1,0,0},	{0,1}, {0,0,0}, {0,0,0}	},
		{	{-1,1,1},	{-1,0,0},	{0,0}, {0,0,0}, {0,0,0}	},
		{	{-1,1,-1},	{-1,0,0},	{1,0}, {0,0,0}, {0,0,0}	},
		{	{-1,-1,-1},	{-1,0,0},	{1,1}, {0,0,0}, {0,0,0}	},
										 
		{	{1,-1,-1},	{1,0,0}	,	{0,1}, {0,0,0}, {0,0,0}	},
		{	{1,1,-1},	{1,0,0}	,	{0,0}, {0,0,0}, {0,0,0}	},
		{	{1,1,1},	{1,0,0}	,	{1,0}, {0,0,0}, {0,0,0}	},
		{	{1,-1,1},	{1,0,0}	,	{1,1}, {0,0,0}, {0,0,0}	}
	};

	DIndex indices[] = {
		0,1,2,
		0,2,3,

		4,5,6,
		4,6,7,

		8,9,10,
		8,10,11,

		12,13,14,
		12,14,15,

		16,17,18,
		16,18,19,

		20,21,22,
		20,22,23
	};

#pragma endregion end of static data


	Mesh* newBox = new Mesh(createVertexBuffer(&mesh[0],
							sizeof(mesh)/sizeof(PNTTBVertex)),
							createIndexBuffer(&indices[0],
							sizeof(indices)/sizeof(DIndex)));

	return newBox;
}

LightMesh* BufferFactory::createLightBoxMesh()
{
#pragma region static data
	PVertex mesh[]= {
		{ { -1, -1, -1 } },
		{ { -1,  1, -1 } },
		{ {  1,  1, -1 } } ,
		{ {  1, -1, -1 } },

		{ { -1, -1,  1 },},
		{ {  1, -1,  1 },},
		{ {  1,  1,  1 },},
		{ { -1,  1,  1 },},

		{ { -1,  1, -1 },},
		{ { -1,  1,  1 },},
		{ {  1,  1,  1 },},
		{ {  1,  1, -1 },},

		{ { -1, -1, -1 },},
		{ {  1, -1, -1 },},
		{ {  1, -1,  1 },},
		{ { -1, -1,  1 },},

		{ { -1, -1,  1 },},
		{ { -1,  1,  1 },},
		{ { -1,  1, -1 },},
		{ { -1, -1, -1 },},

		{ {  1, -1, -1 },},
		{ {  1,  1, -1 },},
		{ {  1,  1,  1 },},
		{ {  1, -1,  1 },}
	};

	DIndex indices[] = {
		0,1,2,
		0,2,3,

		4,5,6,
		4,6,7,

		8,9,10,
		8,10,11,

		12,13,14,
		12,14,15,

		16,17,18,
		16,18,19,

		20,21,22,
		20,22,23
	};

#pragma endregion end of static data
	int indicesCnt = sizeof(indices) / sizeof(DIndex);
	Buffer<DIndex>* indexBuffer = createIndexBuffer( &indices[0], indicesCnt );

	int vertexCnt = sizeof(mesh) / sizeof(PVertex);
	Buffer<PVertex>* vertexBuffer = createVertexBuffer( &mesh[0], vertexCnt );

	LightMesh* newBox = new LightMesh( vertexBuffer, indexBuffer );

	return newBox;
}

Mesh* BufferFactory::createSphereMesh()
{
	AglSphereMesh sphereMesh;
	int vertexCount = sphereMesh.positions.size();
	int indexCount	= sphereMesh.indices.size();

	PNTTBVertex* mesh = new PNTTBVertex[vertexCount];
	for (int vertexIndex = 0; vertexIndex < vertexCount; vertexIndex++)
	{
		mesh[vertexIndex].pos[0] = sphereMesh.positions[vertexIndex].x;
		mesh[vertexIndex].pos[1] = sphereMesh.positions[vertexIndex].y;
		mesh[vertexIndex].pos[2] = sphereMesh.positions[vertexIndex].z;

		mesh[vertexIndex].normal[0] = sphereMesh.normals[vertexIndex].x;
		mesh[vertexIndex].normal[1] = sphereMesh.normals[vertexIndex].y;
		mesh[vertexIndex].normal[2] = sphereMesh.normals[vertexIndex].z;

		mesh[vertexIndex].texCoord[0] = sphereMesh.texCoord[vertexIndex].x;
		mesh[vertexIndex].texCoord[1] = sphereMesh.texCoord[vertexIndex].y;

		mesh[vertexIndex].tangent[0] = 0;
		mesh[vertexIndex].tangent[1] = 0;
		mesh[vertexIndex].tangent[2] = 0;

		mesh[vertexIndex].binormal[0] = 0;
		mesh[vertexIndex].binormal[1] = 0;
		mesh[vertexIndex].binormal[2] = 0;
	}

	DIndex* indices = new DIndex[indexCount];
	memcpy(indices, &sphereMesh.indices[0], sizeof(unsigned int) * indexCount);

	Mesh* newSphere = new Mesh( createVertexBuffer(mesh, vertexCount),
								createIndexBuffer(indices, indexCount));

	delete [] mesh;
	delete [] indices;

	return newSphere;
}

Mesh* BufferFactory::createMeshFromPNTTBVerticesAndIndices( 
	int p_numVertices, PNTTBVertex* p_vertices, int p_numIndices, DIndex* p_indices )
{
	// Create description for buffer
	BufferConfig::BUFFER_INIT_DESC vertexBufferDesc;
	vertexBufferDesc.ElementSize = sizeof(PNTTBVertex);
	vertexBufferDesc.Usage = BufferConfig::BUFFER_DEFAULT;
	vertexBufferDesc.NumElements = p_numVertices;
	vertexBufferDesc.Type = BufferConfig::VERTEX_BUFFER;
	vertexBufferDesc.Slot = BufferConfig::SLOT0;

	// Create description for buffer
	BufferConfig::BUFFER_INIT_DESC indexBufferDesc;
	indexBufferDesc.ElementSize = sizeof(DIndex);
	indexBufferDesc.Usage = BufferConfig::BUFFER_DEFAULT;
	indexBufferDesc.NumElements = p_numIndices;
	indexBufferDesc.Type = BufferConfig::INDEX_BUFFER;
	indexBufferDesc.Slot = BufferConfig::SLOT0;

	Buffer<PNTTBVertex>* vertexBuffer = new Buffer<PNTTBVertex>( m_device, m_deviceContext,
		p_vertices, vertexBufferDesc );
	Buffer<DIndex>* indexBuffer = new Buffer<DIndex>( m_device, m_deviceContext, p_indices,
		indexBufferDesc );

	Mesh* mesh = new Mesh( vertexBuffer, indexBuffer );

	return mesh;
}

Mesh* BufferFactory::createMeshFromPTVerticesAndIndices( 
	int p_numVertices, PTVertex* p_vertices, int p_numIndices, DIndex* p_indices )
{
	// Create description for buffer
	BufferConfig::BUFFER_INIT_DESC vertexBufferDesc;
	vertexBufferDesc.ElementSize = sizeof(PTVertex);
	vertexBufferDesc.Usage = BufferConfig::BUFFER_DEFAULT;
	vertexBufferDesc.NumElements = p_numVertices;
	vertexBufferDesc.Type = BufferConfig::VERTEX_BUFFER;
	vertexBufferDesc.Slot = BufferConfig::SLOT0;

	// Create description for buffer
	BufferConfig::BUFFER_INIT_DESC indexBufferDesc;
	indexBufferDesc.ElementSize = sizeof(DIndex);
	indexBufferDesc.Usage = BufferConfig::BUFFER_DEFAULT;
	indexBufferDesc.NumElements = p_numIndices;
	indexBufferDesc.Type = BufferConfig::INDEX_BUFFER;
	vertexBufferDesc.Slot = BufferConfig::SLOT0;

	Buffer<PTVertex>* vertexBuffer = new Buffer<PTVertex>( m_device, m_deviceContext,
		p_vertices, vertexBufferDesc );
	Buffer<DIndex>* indexBuffer = new Buffer<DIndex>( m_device, m_deviceContext, p_indices,
		indexBufferDesc );

	//Mesh* mesh = new Mesh( vertexBuffer, indexBuffer );

	return NULL;
}

Mesh* BufferFactory::createMeshFromRaw( void* p_vertexBlob, void* p_indexBlob, 
									   unsigned int p_numberOfVertices, 
									   unsigned int p_numberOfIndices )
{
	Mesh* newMesh = new Mesh(createVertexBuffer(static_cast<PNTTBVertex*>(p_vertexBlob),
												p_numberOfVertices),
							 createIndexBuffer(static_cast<DIndex*>(p_indexBlob),
												p_numberOfIndices) 
							);

	return newMesh;
}

