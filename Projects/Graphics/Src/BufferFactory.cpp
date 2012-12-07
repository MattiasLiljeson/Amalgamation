#include "BufferFactory.h"

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
		{0.0f,1.0f,0.0f,  1.0f},
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

	// Create buffer from config and data
	quadBuffer = new Buffer<PTVertex>(m_device,m_deviceContext,&mesh[0],bufferDesc);

	return quadBuffer;
}

Buffer<InstanceVertex>* BufferFactory::createInstanceBuffer(InstanceVertex* p_instanceList, 
															unsigned int p_numberOfElements)
{
	Buffer<InstanceVertex>* instanceBuffer;

	// Create description for buffer
	BufferConfig::BUFFER_INIT_DESC bufferDesc;
	bufferDesc.ElementSize = sizeof(InstanceVertex);
	bufferDesc.Usage = BufferConfig::BUFFER_DEFAULT;
	bufferDesc.NumElements = p_numberOfElements;
	bufferDesc.Type = BufferConfig::VERTEX_BUFFER;

	// Create buffer from config and data
	instanceBuffer = new Buffer<InstanceVertex>(m_device,m_deviceContext,
											    p_instanceList,bufferDesc);

	return instanceBuffer;
}

Mesh* BufferFactory::createBoxMesh()
{
#pragma region static data
	PTNVertex mesh[]= {
		{	{-1,-1,-1},	{0,1},	{0,0,-1}	},
		{	{-1,1,-1},	{0,0},	{0,0,-1}	},
		{	{1,1,-1},	{1,0},	{0,0,-1}	},
		{	{1,-1,-1},	{1,1},	{0,0,-1}	},

		{	{-1,-1,1},	{1,1},	{0,0,1}		},
		{	{1,-1,1},	{0,1},	{0,0,1}		},
		{	{1,1,1},	{0,0},	{0,0,1}		},
		{	{-1,1,1},	{1,0},	{0,0,1}		},
			
		{	{-1,1,-1},	{0,1},	{0,1,0}		},
		{	{-1,1,1},	{0,0},	{0,1,0}		},
		{	{1,1,1},	{1,0},	{0,1,0}		},
		{	{1,1,-1},	{1,1},	{0,1,0}		},
			
		{	{-1,-1,-1},	{1,1},	{0,-1,0}	},
		{	{1,-1,-1},	{0,1},	{0,-1,0}	},
		{	{1,-1,1},	{0,0},	{0,-1,0}	},
		{	{-1,-1,1},	{1,0},	{0,-1,0}	},
			
		{	{-1,-1,1},	{0,1},	{-1,0,0}	},
		{	{-1,1,1},	{0,0},	{-1,0,0}	},
		{	{-1,1,-1},	{1,0},	{-1,0,0}	},
		{	{-1,-1,-1},	{1,1},	{-1,0,0}	},
			
		{	{1,-1,-1},	{0,1},	{1,0,0}		},
		{	{1,1,-1},	{0,0},	{1,0,0}		},
		{	{1,1,1},	{1,0},	{1,0,0}		},
		{	{1,-1,1},	{1,1},	{1,0,0}		}
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

	// Create description for buffer
	BufferConfig::BUFFER_INIT_DESC vertexBufferDesc;
	vertexBufferDesc.ElementSize = sizeof(PTNVertex);
	vertexBufferDesc.Usage = BufferConfig::BUFFER_DEFAULT;
	vertexBufferDesc.NumElements = sizeof(mesh)/vertexBufferDesc.ElementSize;
	vertexBufferDesc.Type = BufferConfig::VERTEX_BUFFER;

	// Create description for buffer
	BufferConfig::BUFFER_INIT_DESC indexBufferDesc;
	indexBufferDesc.ElementSize = sizeof(DIndex);
	indexBufferDesc.Usage = BufferConfig::BUFFER_DEFAULT;
	indexBufferDesc.NumElements = sizeof(indices)/indexBufferDesc.ElementSize;
	indexBufferDesc.Type = BufferConfig::INDEX_BUFFER;

	Buffer<PTNVertex>* vertexBuffer = new Buffer<PTNVertex>(m_device,m_deviceContext,
		&mesh[0],vertexBufferDesc);
	Buffer<DIndex>* indexBuffer = new Buffer<DIndex>(m_device,m_deviceContext, &indices[0],
		indexBufferDesc);
	
	Mesh* newBox = new Mesh(vertexBuffer,indexBuffer);

	return newBox;
}
