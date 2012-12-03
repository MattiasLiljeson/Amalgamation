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
		{0.0f,1.0f,0.0f,1.0f},
		{0.0f,0.0f,1.0f,1.0f}
	};
	UINT32 elemSz = sizeof(float)*4;

	// set up buffer description: usage, type and size
	BufferConfig::BUFFER_INIT_DESC bufferDesc;
	bufferDesc.ElementSize = elemSz;
	bufferDesc.Usage = BufferConfig::BUFFER_CPU_WRITE_DISCARD;
	bufferDesc.NumElements = sizeof(data)/elemSz;
	bufferDesc.Type = BufferConfig::CONSTANT_BUFFER_PS;

	initConfig = new BufferConfig(bufferDesc);

	// create and return the buffer
	cBuffer = new Buffer<SimpleCBuffer>(m_device,m_deviceContext,&data,initConfig);
	return cBuffer;
}


