#ifndef DEBUGBOX_H
#define DEBUGBOX_H

#include "Utility.h"

// =================================================================================================
//                                         DEBUGBOX
// =================================================================================================

///-------------------------------------------------------------------------------------------------
/// \brief DebugBox contains logic for drawing a box shape
///
/// 
/// Edited By: Anton Andersson, 2012-11-14
///-------------------------------------------------------------------------------------------------
class DebugBox
{
private:
	ID3D11Device*			m_device;
	ID3D11DeviceContext*	m_deviceContext;
	ID3D11Buffer*			m_vb;
	ID3D11Buffer*			m_ib;
public:
	DebugBox(ID3D11Device*, ID3D11DeviceContext*);
	void draw(AglMatrix pWorld, bool pWireframe = false);
};

#endif