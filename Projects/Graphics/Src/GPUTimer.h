#pragma once
#include <d3d11.h>
#include <vector>
#include "D3DException.h"

using namespace std;

// =======================================================================================
//                                      GPUTimer
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	GPUTimer
///        
/// # GPUTimer
/// Detailed description.....
/// Created on: 8-2-2013 
///---------------------------------------------------------------------------------------

class GPUTimer
{
public:
	GPUTimer(ID3D11Device* p_device, ID3D11DeviceContext* p_deviceContext);
	~GPUTimer();
	void Start(unsigned int p_index);
	void Stop(unsigned int p_index);

	double getTheTimeAndReset(unsigned int p_index);
private:
	void initTimers();
public:
	static const UINT64 QUREY_LATENCY = 5;
private:
	struct Timer{
		ID3D11Query*	disjoint;
		ID3D11Query*	start;
		ID3D11Query*	stop;
		bool			queryStarted;
		bool			queryStoped;

		Timer() : queryStoped(false), queryStarted(false) {}
		~Timer(){
			if(start)		start->Release();
			if(stop)		stop->Release();
			if(disjoint)	disjoint->Release();
		}
	};

	vector<Timer*> timers;

	ID3D11Device*			m_device;
	ID3D11DeviceContext*	m_deviceContext;
};