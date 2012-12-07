#include "BufferBase.h"

BufferBase::BufferBase(ID3D11Device* p_device, ID3D11DeviceContext* p_deviceContext, 
					   BufferConfig::BUFFER_INIT_DESC& p_configDesc)
{
	m_config = new BufferConfig(p_configDesc);
	m_device = p_device;
	m_deviceContext = p_deviceContext;
}

BufferBase::~BufferBase()
{
	delete m_config;
	SAFE_RELEASE(m_buffer);
}


void* BufferBase::map()
{
	void* ret = NULL;
	BufferConfig::BUFFER_USAGE usage = m_config->usage;
	if(usage == BufferConfig::BUFFER_CPU_WRITE || 
		usage == BufferConfig::BUFFER_CPU_READ || 
		usage == BufferConfig::BUFFER_CPU_WRITE_DISCARD)
	{
		D3D11_MAPPED_SUBRESOURCE MappedResource;
		UINT32 mapType = 0;

		if(usage == BufferConfig::BUFFER_CPU_READ)	mapType = D3D11_MAP_READ;
		else if(usage == BufferConfig::BUFFER_CPU_WRITE) mapType = D3D11_MAP_WRITE;
		else if(usage == BufferConfig::BUFFER_CPU_WRITE_DISCARD)	mapType = D3D11_MAP_WRITE_DISCARD;

		HRESULT hr = S_OK;
		if(FAILED(hr = m_deviceContext->Map( m_buffer, 0, (D3D11_MAP)mapType, 0,
			&MappedResource)))
		{
			throw D3DException(hr,__FILE__,__FUNCTION__,__LINE__);
		}
		else
		{
			ret = MappedResource.pData;
		}
	}

	return ret;
}
void BufferBase::unmap()
{
	m_deviceContext->Unmap( m_buffer, 0 );
}

void BufferBase::apply( UINT32 misc /*= 0*/ )
{
	HRESULT hr = S_OK;

	switch(m_config->type)
	{
	case BufferConfig::VERTEX_BUFFER:
		{
			UINT32 vertexSize = m_config->elementSize;
			UINT32 offset = 0;
			m_deviceContext->IASetVertexBuffers(misc, 1, &m_buffer, &vertexSize, &offset );
		}
		break;
	case BufferConfig::INDEX_BUFFER:
		{
			m_deviceContext->IASetIndexBuffer(m_buffer, DXGI_FORMAT_R32_UINT, 0);
		}
		break;
	case BufferConfig::CONSTANT_BUFFER_VS:
		{
			m_deviceContext->VSSetConstantBuffers(misc, 1, &m_buffer);
		}
		break;
	case BufferConfig::CONSTANT_BUFFER_GS:
		{
			m_deviceContext->GSSetConstantBuffers(misc, 1, &m_buffer);
		}
		break;
	case BufferConfig::CONSTANT_BUFFER_PS:
		{
			m_deviceContext->PSSetConstantBuffers(misc, 1, &m_buffer);
		}
		break;
	case BufferConfig::CONSTANT_BUFFER_VS_PS:
		{
			m_deviceContext->VSSetConstantBuffers(misc, 1, &m_buffer);
			m_deviceContext->PSSetConstantBuffers(misc, 1, &m_buffer);
		}
		break;	
	case BufferConfig::CONSTANT_BUFFER_VS_GS_PS:
		{
			m_deviceContext->VSSetConstantBuffers(misc, 1, &m_buffer);
			m_deviceContext->GSSetConstantBuffers(misc, 1, &m_buffer);
			m_deviceContext->PSSetConstantBuffers(misc, 1, &m_buffer);
		}
		break;	
	case BufferConfig::CONSTANT_BUFFER_GS_PS:
		{
			m_deviceContext->GSSetConstantBuffers(misc, 1, &m_buffer);
			m_deviceContext->PSSetConstantBuffers(misc, 1, &m_buffer);
		}
		break;	
	case BufferConfig::CONSTANT_BUFFER_VS_GS:
		{
			m_deviceContext->VSSetConstantBuffers(misc, 1, &m_buffer);
			m_deviceContext->GSSetConstantBuffers(misc, 1, &m_buffer);
		}
		break;	
	case BufferConfig::CONSTANT_BUFFER_ALL:
		{
			m_deviceContext->VSSetConstantBuffers(misc, 1, &m_buffer);
			m_deviceContext->GSSetConstantBuffers(misc, 1, &m_buffer);
			m_deviceContext->HSSetConstantBuffers(misc, 1, &m_buffer);
			m_deviceContext->DSSetConstantBuffers(misc, 1, &m_buffer);
			m_deviceContext->PSSetConstantBuffers(misc, 1, &m_buffer);
		}
		break;
	default:
		hr = E_FAIL;
		break;
	};
}

/*
ID3D11Buffer* BufferBase::getBufferPointer()
{
	return m_buffer;
}
*/

UINT32 BufferBase::getElementSize()
{
	return m_config->elementSize;
}

UINT32 BufferBase::getElementCount()
{
	return m_config->elementCount;
}

void BufferBase::init(void* p_initData )
{
	HRESULT hr = S_OK;
	if(p_initData)
	{
		D3D11_SUBRESOURCE_DATA data;
		data.pSysMem = p_initData;
		hr = m_device->CreateBuffer(m_config->getBufferDesc(), &data, &m_buffer);
	}
	else
	{
		hr = m_device->CreateBuffer(m_config->getBufferDesc(), NULL, &m_buffer);
	}

	if(FAILED(hr))
	{
		throw D3DException(hr,__FILE__,__FUNCTION__,__LINE__);
	}
}
