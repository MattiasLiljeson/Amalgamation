// =======================================================================================
//                                    InputLayoutElementStruct
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Used to create a input layout desc
///        
/// # InputLayoutStruct
/// This struct is only for the use of creating a input layout and nothing else. It avoids
/// the need to change number of input elements when calling CreateInputLayout. A dynamic
/// vector can thus be used instead of an array.
///
/// vector<InputLayoutElementStruct> inputLayoutData;
///
/// inputLayoutData.push_back(InputLayoutElementStruct("POSITION", 0, 
///		DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,
///		D3D11_INPUT_PER_VERTEX_DATA, 0));
/// inputLayoutData.push_back(InputLayoutElementStruct("TEXCOORD", 0, 
///		DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, 
///		D3D11_INPUT_PER_VERTEX_DATA, 0));
/// 
/// Created on: 28-11-2012 
///---------------------------------------------------------------------------------------
#pragma once

#include <d3d11.h>

struct InputLayoutElementStruct : public  D3D11_INPUT_ELEMENT_DESC
{
	/************************************************************************/
	/* !!!No virtual methods in this struct!!! Size cannot be altered		*/
	/************************************************************************/
	InputLayoutElementStruct( LPCSTR p_semanticName, UINT p_semanticIndex, 
		DXGI_FORMAT p_format, UINT p_inputSlot, UINT p_alignedByteOffset, 
		D3D11_INPUT_CLASSIFICATION p_inputSlotClass, UINT p_instanceDataStepRate)
	{
		SemanticName			= p_semanticName;
		SemanticIndex			= p_semanticIndex;
		Format					= p_format;
		InputSlot				= p_inputSlot;
		AlignedByteOffset		= p_alignedByteOffset;
		InputSlotClass			= p_inputSlotClass;
		InstanceDataStepRate	= p_instanceDataStepRate;
	}
};