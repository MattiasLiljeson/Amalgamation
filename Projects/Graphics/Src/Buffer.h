// =======================================================================================
//                                      Buffer
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Template buffer class that handles mapping/unmapping correctly 
/// based on template type.
///        
/// # Buffer
/// Detailed description.....
/// Created on: 30-11-2012 
///---------------------------------------------------------------------------------------
#pragma once

#include "BufferBase.h"

template <class T>
class Buffer : public BufferBase
{
public:
	Buffer(ID3D11Device* p_device, ID3D11DeviceContext* p_deviceContext, 
		T* p_initData, BufferConfig::BUFFER_INIT_DESC& p_configDesc)
		: BufferBase(p_device, p_deviceContext, p_configDesc)
	{
		accessBuffer = *p_initData;
		init(static_cast<void*>(p_initData));
	}
	virtual ~Buffer() {}

	///-----------------------------------------------------------------------------------
	/// Update buffer on GPU from CPU representation (accessBuffer)
	/// \returns void
	///-----------------------------------------------------------------------------------
	void update()
	{
		void* bufferGenericData = map();
		T* buf = static_cast<T*>(bufferGenericData);
		*buf = accessBuffer;
		unmap();
	}

	///
	/// Buffer for CPU access, gets copied to GPU(map/write/unmap) on update
	///
	T accessBuffer;

protected:
private:
};