#include "GPUTimer.h"

GPUTimer::GPUTimer(ID3D11Device* p_device, ID3D11DeviceContext* p_deviceContext)
{

	m_device = p_device;
	m_deviceContext = p_deviceContext;
	for (unsigned int i = 0; i < QUREY_LATENCY; i++){
		timers.push_back(new Timer());
	}
	initTimers();
}

GPUTimer::~GPUTimer()
{
	for (unsigned int i = 0 ; i < timers.size(); i++)
	{
		delete timers[i];
	}
	timers.clear();
}

void GPUTimer::initTimers()
{
	HRESULT hr = S_OK;

	for(unsigned int i = 0 ; i < QUREY_LATENCY; i++){
		D3D11_QUERY_DESC desc;
		desc.Query = D3D11_QUERY_TIMESTAMP_DISJOINT;
		desc.MiscFlags = 0;
		if(FAILED(hr = m_device->CreateQuery(&desc, &timers[i]->disjoint))){
			throw D3DException(hr, __FILE__,__FUNCTION__,__LINE__);
		}
		desc.Query = D3D11_QUERY_TIMESTAMP;
		if(FAILED(hr = m_device->CreateQuery(&desc, &timers[i]->start))){
			throw D3DException(hr, __FILE__,__FUNCTION__,__LINE__);
		}
		if(FAILED(hr = m_device->CreateQuery(&desc, &timers[i]->stop))){
			throw D3DException(hr, __FILE__,__FUNCTION__,__LINE__);
		}
	}
}

void GPUTimer::Start(unsigned int p_index)
{
	if(timers[p_index]->queryStarted == true)
		return;

	m_deviceContext->Begin(timers[p_index]->disjoint);
	m_deviceContext->End(timers[p_index]->start);
	timers[p_index]->queryStarted = true;
}

void GPUTimer::Stop(unsigned int p_index)
{
	if(timers[p_index]->queryStoped == true)
		return;

	m_deviceContext->End(timers[p_index]->stop);
	m_deviceContext->End(timers[p_index]->disjoint);
	timers[p_index]->queryStoped = true;
}

double GPUTimer::getTheTimeAndReset(unsigned int p_index)
{
	if(timers[p_index]->queryStoped == true ){
		timers[p_index]->queryStarted = false;
		timers[p_index]->queryStoped = false;
		UINT64 startTime = 0;
		while(m_deviceContext->GetData(timers[p_index]->start, &startTime, sizeof(startTime), 0) != S_OK);

		UINT64 endTime = 0;
		while(m_deviceContext->GetData(timers[p_index]->stop, &endTime, sizeof(endTime), 0) != S_OK);

		D3D11_QUERY_DATA_TIMESTAMP_DISJOINT disjointData;
		while(m_deviceContext->GetData(timers[p_index]->disjoint, &disjointData, sizeof(disjointData), 0) != S_OK);

		double time = -1.0f;
		if(disjointData.Disjoint == FALSE)
		{
			UINT64 delta = endTime - startTime;
			double frequency = static_cast<double>(disjointData.Frequency);
			time = (delta / frequency) * 1000.0f;
		}
		return time;
	}
	return 0;
}
