#include "GPUTimer.h"

GPUTimer::GPUTimer(ID3D11Device* p_device, ID3D11DeviceContext* p_deviceContext)
{

	m_device = p_device;
	m_deviceContext = p_deviceContext;
	
}

GPUTimer::~GPUTimer()
{
	ProfileMap::iterator iterator;
	for(iterator = m_profileMap.begin(); iterator != m_profileMap.end(); iterator++){
		delete iterator->second;
	}
}

void GPUTimer::addProfile( string p_profile ){
	Profile* newProfile = new Profile();

	for (unsigned int i = 0; i < QUREY_LATENCY; i++){
			Timer* newTimer = new Timer(m_device);
			newProfile->timers.push_back(newTimer);
	}
	m_profileMap.insert( pair<string,Profile*>(p_profile,newProfile));
}

void GPUTimer::Start(string p_profile, int p_frame){

	Profile* currentProfile = m_profileMap[p_profile];

	if(currentProfile->timers[p_frame]->queryStarted == true)
		return;

	m_deviceContext->Begin(currentProfile->timers[p_frame]->disjoint);
	m_deviceContext->End(currentProfile->timers[p_frame]->start);
	currentProfile->timers[p_frame]->queryStarted = true;
}

void GPUTimer::Stop(string p_profile, int p_frame){
	Profile* currentProfile = m_profileMap[p_profile];

	if(currentProfile->timers[p_frame]->queryStoped == true)
		return;

	m_deviceContext->End(currentProfile->timers[p_frame]->stop);
	m_deviceContext->End(currentProfile->timers[p_frame]->disjoint);
	currentProfile->timers[p_frame]->queryStoped = true;
}

double GPUTimer::getTheTimeAndReset(string p_profile, int p_frame){

	Profile* currentProfile = m_profileMap[p_profile];

	if(currentProfile->timers[p_frame]->queryStoped == true ){
		currentProfile->timers[p_frame]->queryStarted = false;
		currentProfile->timers[p_frame]->queryStoped = false;
		UINT64 startTime = 0;
		while(m_deviceContext->GetData(currentProfile->timers[p_frame]->start, &startTime, sizeof(startTime), 0) != S_OK);

		UINT64 endTime = 0;
		while(m_deviceContext->GetData(currentProfile->timers[p_frame]->stop, &endTime, sizeof(endTime), 0) != S_OK);

		D3D11_QUERY_DATA_TIMESTAMP_DISJOINT disjointData;
		while(m_deviceContext->GetData(currentProfile->timers[p_frame]->disjoint, &disjointData, sizeof(disjointData), 0) != S_OK);

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
