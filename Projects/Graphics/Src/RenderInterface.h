#pragma once

// =======================================================================================
//                                      RenderInterface
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	A simple interface that allows for system to be rendered.
///        
/// # RenderInterface
/// Detailed description.....
/// Created on: 22-1-2013 
///---------------------------------------------------------------------------------------

class RenderInterface
{
public:
	RenderInterface(){}
	virtual ~RenderInterface(){}
	virtual void render()=0;
};