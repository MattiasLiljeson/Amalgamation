// =======================================================================================
//                                      BufferBase
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Base class that implement basic buffer functionality for
/// reading/writing to buffers.
///        
/// # BufferBase
/// Detailed description.....
/// Created on: 30-11-2012 
///---------------------------------------------------------------------------------------
#pragma once

#include <d3d11.h>
#include "BufferConfig.h"

class BufferBase
{
public:
	BufferBase(ID3D11Device* p_device, ID3D11DeviceContext* p_deviceContext, 
			   BufferConfig::BUFFER_INIT_DESC& p_configDesc);
	virtual ~BufferBase();
	///-----------------------------------------------------------------------------------
	/// Apply changes made to buffer.
	/// \param misc
	/// \return void
	///-----------------------------------------------------------------------------------
	void			apply(UINT32 misc = 0);

	ID3D11Buffer*	getBufferPointer();
	const BufferConfig*	getBufferConfigPointer();

	UINT32			getElementSize();
	UINT32			getElementCount();
protected:
	void			init(void* p_initData);
	void*			map();
	void			unmap();
	
	BufferConfig*	m_config;
private:
	ID3D11Buffer*	m_buffer;

	ID3D11Device*			m_device;
	ID3D11DeviceContext*	m_deviceContext;
};