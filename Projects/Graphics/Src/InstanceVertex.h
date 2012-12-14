#pragma once

// =======================================================================================
//                                      InstanceVertex
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Data for an instance. Transformation, colour, etc.
/// TODO: Evaluate how it should be initialized.       
/// # InstanceVertex
/// Detailed description.....
/// Created on: 7-12-2012 
///---------------------------------------------------------------------------------------

struct InstanceVertex
{
	/*
	InstanceVertex(float* p_4x4Init)
	{
		for (int i=0;i<16;i++) // find a better method to do init
			worldTransform[i] = p_4x4Init[i];
	}
	InstanceVertex(float p_4x4Init[])
	{
		worldTransform = p_4x4Init;
	}*/

	float worldTransform[16];
};