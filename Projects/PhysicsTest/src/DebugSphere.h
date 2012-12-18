#ifndef DEBUGSPHERE_H
#define DEBUGSPHERE_H

#include "VertexStructures.h"
#include "Utility.h"

class DebugSphere
{
private:
	ID3D11Device*			m_device;
	ID3D11DeviceContext*	m_deviceContext;
	ID3D11Buffer*			m_vb;
	ID3D11Buffer*			m_ib;
	int						m_indexCount;
public:
	DebugSphere(int, ID3D11Device*, ID3D11DeviceContext*);
	void draw(AglMatrix pWorld, bool pWireframe = false);
};

#endif