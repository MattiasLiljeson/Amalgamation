#include "GPUTimer.h"

GPUTimer::GPUTimer(ID3D11Device* p_device, ID3D11DeviceContext* p_deviceContext)
{

	m_device = p_device;
	m_deviceContext = p_deviceContext;
	m_currentFrame = 0;
	m_measureFrame = QUREY_LATENCY-1;
}

GPUTimer::~GPUTimer()
{
	ProfileMap::iterator iterator;
	for(iterator = m_profileMap.begin(); iterator != m_profileMap.end(); iterator++){
		delete iterator->second;
	}
}

void GPUTimer::addProfile( const string& p_profile ){
	Profile* newProfile = new Profile();

	for (unsigned int i = 0; i < QUREY_LATENCY; i++){
			Timer* newTimer = new Timer(m_device);
			newProfile->timers.push_back(newTimer);
	}
	m_profileMap.insert( pair<const string&,Profile*>(p_profile,newProfile));
}

void GPUTimer::Start(const string& p_profile){

	Profile* currentProfile = m_profileMap[p_profile];

	if(currentProfile->timers[m_measureFrame]->queryStarted == true)
		return;

	m_deviceContext->Begin(currentProfile->timers[m_measureFrame]->disjoint);
	m_deviceContext->End(currentProfile->timers[m_measureFrame]->start);
	currentProfile->timers[m_measureFrame]->queryStarted = true;
}

void GPUTimer::Stop(const string& p_profile){
	Profile* currentProfile = m_profileMap[p_profile];

	if(currentProfile->timers[m_measureFrame]->queryStoped == true)
		return;

	m_deviceContext->End(currentProfile->timers[m_measureFrame]->stop);
	m_deviceContext->End(currentProfile->timers[m_measureFrame]->disjoint);
	currentProfile->timers[m_measureFrame]->queryStoped = true;
}

double GPUTimer::getTheTimeAndReset(const string& p_profile){

	Profile* currentProfile = m_profileMap[p_profile];

	if(currentProfile->timers[m_currentFrame]->queryStoped == true ){
		currentProfile->timers[m_currentFrame]->queryStarted = false;
		currentProfile->timers[m_currentFrame]->queryStoped = false;
		UINT64 startTime = 0;
		while(m_deviceContext->GetData(currentProfile->timers[m_currentFrame]->start, 
			&startTime, sizeof(startTime), 0) != S_OK);

		UINT64 endTime = 0;
		while(m_deviceContext->GetData(currentProfile->timers[m_currentFrame]->stop, 
			&endTime, sizeof(endTime), 0) != S_OK);

		D3D11_QUERY_DATA_TIMESTAMP_DISJOINT disjointData;
		while(m_deviceContext->GetData(currentProfile->timers[m_currentFrame]->disjoint, 
			&disjointData, sizeof(disjointData), 0) != S_OK);

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

void GPUTimer::tick()
{
	m_currentFrame = (m_currentFrame+1 >= QUREY_LATENCY) ? 0 : m_currentFrame+=1;
	m_measureFrame = (m_measureFrame+1 >= QUREY_LATENCY) ? 0 : m_measureFrame+=1;
}
