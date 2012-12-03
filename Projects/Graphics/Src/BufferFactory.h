// =======================================================================================
//                                      BufferFactory
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Factory used to construct various buffers
///        
/// # BufferFactory
/// Detailed description.....
/// Created on: 3-12-2012 
///---------------------------------------------------------------------------------------
#pragma once

#include "Buffer.h"
#include "CBuffers.h"
#include "PTVertex.h"
// #include "TextureBuffers.h"
// #include "VertexBuffers.h"
// etc?


class BufferFactory
{
public:
	BufferFactory(ID3D11Device* p_device, ID3D11DeviceContext* p_deviceContext);
	virtual ~BufferFactory();

	///-----------------------------------------------------------------------------------
	/// Sets up and return a buffer object of type SimpleCBuffer
	/// \returns SimpleCBuffer*
	///-----------------------------------------------------------------------------------
	Buffer<SimpleCBuffer>* createSimpleCBuffer();

	Buffer<PTVertex>* createFullScreenQuadBuffer();

protected:
private:
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
};