#pragma once

#include "ShaderBase.h"

// =======================================================================================
//                                      DeferredPostProcessing
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	
///        
/// # DeferredPostProcessing
/// Detailed description.....
/// Created on: 13-3-2013 
///---------------------------------------------------------------------------------------

class DeferredPostProcessing : public ShaderBase
{
public:
	DeferredPostProcessing();
	virtual ~DeferredPostProcessing();
	void apply();
	void setPostProcessingBuffer();
protected:
private:

};