// =======================================================================================
//                                      BufferConfig
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Configuration struct for buffers. Defines read/write settings, size and
/// element count.
///        
/// # Buffer
/// Detailed description.....
/// Created on: 30-11-2012 
///---------------------------------------------------------------------------------------
#pragma once

#include <d3d11.h>
#include "D3DException.h"
#include "D3DUtil.h"

struct BufferConfig
{
	enum BUFFER_TYPE
	{
		VERTEX_BUFFER,
		INDEX_BUFFER,
		CONSTANT_BUFFER_VS,
		CONSTANT_BUFFER_GS,
		CONSTANT_BUFFER_PS,
		CONSTANT_BUFFER_VS_PS,
		CONSTANT_BUFFER_VS_GS_PS,
		CONSTANT_BUFFER_GS_PS,
		CONSTANT_BUFFER_VS_GS,
		CONSTANT_BUFFER_ALL,
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
	};

	BufferConfig(BUFFER_INIT_DESC& p_initDesc);

	BUFFER_TYPE		type;
	BUFFER_USAGE	usage;

	UINT32			elementSize;
	UINT32			elementCount;

	const D3D11_BUFFER_DESC* getBufferDesc() const;
private:
	D3D11_BUFFER_DESC m_bufferDesc;
};