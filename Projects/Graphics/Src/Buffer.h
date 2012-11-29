// =======================================================================================
//                                      Buffer
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Wrapper for buffer objects
///        
/// # Buffer
/// Detailed description.....
/// Created on: 28-11-2012 
///---------------------------------------------------------------------------------------
#pragma once

#include <d3d11.h>
#include "D3DException.h"
#include "Util.h"

class Buffer
{
public:

	enum BUFFER_TYPE
	{
		VERTEX_BUFFER,
		INDEX_BUFFER,
		CONSTANT_BUFFER_VS,
		CONSTANT_BUFFER_GS,
		CONSTANT_BUFFER_PS,
		BUFFER_TYPE_COUNT
	};


	enum BUFFER_USAGE
	{
		BUFFER_DEFAULT,
		BUFFER_STREAM_OUT_TARGET,
		BUFFER_CPU_WRITE,
		BUFFER_CPU_WRITE_DISCARD,
		BUFFER_CPU_READ,
		BUFFER_USAGE_COUNT
	};

	struct BUFFER_INIT_DESC
	{
		BUFFER_TYPE		Type;
		UINT32			NumElements;
		UINT32			ElementSize;
		BUFFER_USAGE	Usage;
		void*			InitData;

		BUFFER_INIT_DESC()
		{
			InitData = NULL;
		}
	};


	Buffer(ID3D11Device* p_device, ID3D11DeviceContext* p_deviceContext, 
		BUFFER_INIT_DESC& p_initDesc);
	virtual ~Buffer();
	void*			map();
	void			unmap();
	void			apply(UINT32 misc = 0);
	ID3D11Buffer*	getBufferPointer();
	UINT32			getVertexSize();
	UINT32			getElementCount();
private:
	ID3D11Buffer*	m_buffer;
	BUFFER_TYPE		m_type;
	BUFFER_USAGE	m_usage;

	UINT32			m_elementSize;
	UINT32			m_elementCount;

	ID3D11Device*			m_device;
	ID3D11DeviceContext*	m_deviceContext;
};