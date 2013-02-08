#include "GPUTimer.h"

GPUTimer::GPUTimer(ID3D11Device* p_device, ID3D11DeviceContext* p_deviceContext)
{

	m_device = p_device;
	m_deviceContext = p_deviceContext;
	initTimers();
}

GPUTimer::~GPUTimer()
{
	if(m_start)		m_start->Release();
	if(m_stop)		m_stop->Release();
	if(m_disjoint)	m_disjoint->Release();
}

void GPUTimer::initTimers()
{
	HRESULT hr = S_OK;
	D3D11_QUERY_DESC desc;
	desc.Query = D3D11_QUERY_TIMESTAMP_DISJOINT;
	desc.MiscFlags = 0;
	if(FAILED(hr = m_device->CreateQuery(&desc, &m_disjoint))){
		throw D3DException(hr, __FILE__,__FUNCTION__,__LINE__);
	}

	desc.Query = D3D11_QUERY_TIMESTAMP;
	if(FAILED(hr = m_device->CreateQuery(&desc, &m_start))){
		throw D3DException(hr, __FILE__,__FUNCTION__,__LINE__);
	}
	if(FAILED(hr = m_device->CreateQuery(&desc, &m_stop))){
		throw D3DException(hr, __FILE__,__FUNCTION__,__LINE__);
	}
}

void GPUTimer::Start()
{
	m_deviceContext->Begin(m_disjoint);

	m_deviceContext->End(m_start);
}

void GPUTimer::Stop()
{
	m_deviceContext->End(m_stop);

	m_deviceContext->End(m_disjoint);
}

double GPUTimer::GetTime()
{
	UINT64 startTime = 0;
	while(m_deviceContext->GetData(m_start, &startTime, sizeof(startTime), 0) != S_OK);

	UINT64 endTime = 0;
	while(m_deviceContext->GetData(m_stop, &endTime, sizeof(endTime), 0) != S_OK);

	D3D11_QUERY_DATA_TIMESTAMP_DISJOINT disjointData;
	while(m_deviceContext->GetData(m_disjoint, &disjointData, sizeof(disjointData), 0) != S_OK);

	double time = -1.0f;
	if(disjointData.Disjoint == FALSE)
	{
		UINT64 delta = endTime - startTime;
		double frequency = static_cast<double>(disjointData.Frequency);
		time = (delta / frequency) * 1000.0f;
	}

	return time;
}
