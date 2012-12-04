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
	BufferConfig* initConfig  = NULL;
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

	initConfig = new BufferConfig(bufferDesc);

	// create and return the buffer
	cBuffer = new Buffer<SimpleCBuffer>(m_device,m_deviceContext,&data,initConfig);
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

	// Store description in config object
	BufferConfig* initConfig = new BufferConfig(bufferDesc);

	// Create buffer from config and data
	quadBuffer = new Buffer<PTVertex>(m_device,m_deviceContext,&mesh[0],initConfig);

	return quadBuffer;
}


