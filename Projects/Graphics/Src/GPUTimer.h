#pragma once
#include <d3d11.h>
#include <vector>
#include "D3DException.h"
#include <string>
#include <map>

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
	void addProfile(string p_profile);
	void Start(string p_profile, int p_frame);
	void Stop(string p_profile, int p_frame);

	double getTheTimeAndReset(string p_profile, int p_frame);
public:
	static const UINT64 QUREY_LATENCY = 5;
private:

	struct Timer{
		ID3D11Query*	disjoint;
		ID3D11Query*	start;
		ID3D11Query*	stop;
		bool			queryStarted;
		bool			queryStoped;

		Timer(ID3D11Device* p_device){
			queryStoped = false;
			queryStarted = false;

			HRESULT hr = S_OK;
			D3D11_QUERY_DESC desc;
			desc.Query = D3D11_QUERY_TIMESTAMP_DISJOINT;
			desc.MiscFlags = 0;
			if(FAILED(hr = p_device->CreateQuery(&desc, &disjoint))){
				throw D3DException(hr, __FILE__,__FUNCTION__,__LINE__);
			}
			desc.Query = D3D11_QUERY_TIMESTAMP;
			if(FAILED(hr = p_device->CreateQuery(&desc, &start))){
				throw D3DException(hr, __FILE__,__FUNCTION__,__LINE__);
			}
			if(FAILED(hr = p_device->CreateQuery(&desc, &stop))){
				throw D3DException(hr, __FILE__,__FUNCTION__,__LINE__);
			}
		}
		~Timer(){
			if(start)		start->Release();
			if(stop)		stop->Release();
			if(disjoint)	disjoint->Release();
		}
	};
	struct Profile 
	{
		vector<Timer*> timers;
		~Profile(){
			for (unsigned int i = 0 ; i < QUREY_LATENCY; i++){
				delete timers[i];
			}
			
		}
	};
	typedef std::map<std::string,Profile*> ProfileMap;
	ProfileMap m_profileMap;

	ID3D11Device*			m_device;
	ID3D11DeviceContext*	m_deviceContext;
};