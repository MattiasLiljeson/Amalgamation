#include "Buffer.h"

Buffer::Buffer( ID3D11Device* p_device, ID3D11DeviceContext* p_deviceContext, 
			   BUFFER_INIT_DESC& p_initDesc )
{
	m_device = p_device;
	m_deviceContext = p_deviceContext;
	m_buffer = NULL;

	D3D11_BUFFER_DESC bufferDesc;

	m_type = p_initDesc.Type;
	switch(m_type)
	{
	case VERTEX_BUFFER:
		{
			bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

			if(p_initDesc.Usage == BUFFER_STREAM_OUT_TARGET)
				bufferDesc.BindFlags |= D3D11_BIND_STREAM_OUTPUT;
		}
		break;
	case INDEX_BUFFER:
		{
			bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		}
		break;
	case CONSTANT_BUFFER_VS:
	case CONSTANT_BUFFER_GS:
	case CONSTANT_BUFFER_PS:
		{
			bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		}
		break;
	default:
		throw D3DException("Unknown buffer type.",__FILE__,__FUNCTION__,__LINE__);
		break;
	};

	m_usage = p_initDesc.Usage;
	m_elementSize = p_initDesc.ElementSize;
	m_elementCount = p_initDesc.NumElements;

	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;

	if(m_usage == BUFFER_CPU_READ)
	{
		bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufferDesc.CPUAccessFlags |= D3D11_CPU_ACCESS_READ;
	}
	else if(m_usage == BUFFER_CPU_WRITE)
	{
		bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufferDesc.CPUAccessFlags |= D3D11_CPU_ACCESS_WRITE;
	}
	else if(m_usage == BUFFER_CPU_WRITE_DISCARD)
	{
		bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufferDesc.CPUAccessFlags |= D3D11_CPU_ACCESS_WRITE;
	}

	//Desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.MiscFlags = 0;
	bufferDesc.ByteWidth = p_initDesc.NumElements * p_initDesc.ElementSize;

	//set at least 16 bytes
	if(bufferDesc.ByteWidth < 16)
		bufferDesc.ByteWidth = 16;

	HRESULT hr = S_OK;
	if(p_initDesc.InitData)
	{
		D3D11_SUBRESOURCE_DATA data;
		data.pSysMem = p_initDesc.InitData;
		hr = m_device->CreateBuffer(&bufferDesc, &data, &m_buffer);
	}
	else
	{
		hr = m_device->CreateBuffer(&bufferDesc, NULL, &m_buffer);
	}

	if(FAILED(hr))
	{
		throw D3DException(hr,__FILE__,__FUNCTION__,__LINE__);
		//MessageBox(NULL, "Unable to create buffer.", "Slenda Error", MB_ICONERROR | MB_OK);
	}

}

Buffer::~Buffer()
{
	SAFE_RELEASE(m_buffer);
}

void* Buffer::map()
{
	void* ret = NULL;
	if(m_usage == BUFFER_CPU_WRITE || m_usage == BUFFER_CPU_READ || m_usage == BUFFER_CPU_WRITE_DISCARD)
	{
		D3D11_MAPPED_SUBRESOURCE MappedResource;
		UINT32 mapType = 0;

		if(m_usage == BUFFER_CPU_READ)	mapType = D3D11_MAP_READ;
		else if(m_usage == BUFFER_CPU_WRITE) mapType = D3D11_MAP_WRITE;
		else if(m_usage == BUFFER_CPU_WRITE_DISCARD)	mapType = D3D11_MAP_WRITE_DISCARD;

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
void Buffer::unmap()
{
	m_deviceContext->Unmap( m_buffer, 0 );
}

void Buffer::apply( UINT32 misc /*= 0*/ )
{
	HRESULT hr = S_OK;

	switch(m_type)
	{
	case VERTEX_BUFFER:
		{
			UINT32 vertexSize = m_elementSize;
			UINT32 offset = 0;
			m_deviceContext->IASetVertexBuffers(misc, 1, &m_buffer, &vertexSize, &offset );
		}
		break;
	case INDEX_BUFFER:
		{
			m_deviceContext->IASetIndexBuffer(m_buffer, DXGI_FORMAT_R32_UINT, 0);
		}
		break;
	case CONSTANT_BUFFER_VS:
		{
			m_deviceContext->VSSetConstantBuffers(misc, 1, &m_buffer);
		}
		break;
	case CONSTANT_BUFFER_GS:
		{
			m_deviceContext->GSSetConstantBuffers(misc, 1, &m_buffer);
		}
		break;
	case CONSTANT_BUFFER_PS:
		{
			m_deviceContext->PSSetConstantBuffers(misc, 1, &m_buffer);
		}
		break;
	default:
		hr = E_FAIL;
		break;
	};
}

ID3D11Buffer* Buffer::getBufferPointer()
{
	return m_buffer;
}

UINT32 Buffer::getVertexSize()
{
	return m_elementSize;
}

UINT32 Buffer::getElementCount()
{
	return m_elementCount;
}