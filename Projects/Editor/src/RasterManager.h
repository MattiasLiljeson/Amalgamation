#ifndef RASTERMANAGER_H
#define RASTERMANAGER_H

#include "Utility.h"

class RasterManager
{
private:
	static RasterManager* s_instance;
private:
	ID3D11Device*				m_device;
	ID3D11DeviceContext*		m_deviceContext; 
	ID3D11RasterizerState*		m_standardState;
	ID3D11RasterizerState*		m_invertedState;
	ID3D11RasterizerState*		m_wireframeState;

	ID3D11DepthStencilState*	mStandardDepthStencil;
	ID3D11DepthStencilState*	mNoCullDepthStencil;
private:
	RasterManager();
	~RasterManager();

public:
	static RasterManager* getInstance();
	static void cleanup();

public:
	void initialize(ID3D11Device* p_device, ID3D11DeviceContext* p_deviceContext);
	void setStandardState();
	void setInvertedState();
	void setWireframeState();

	void dsSetStandard();
	void dsSetNoCull();
};

#endif