#pragma once

#include <d3d11.h>
#include <vector>
#include "RenderStateEnums.h"

using namespace std;

struct RasterizerFillMode;
struct RasterizerCullMode;
struct RasterizerFaceVertexOrder;
struct BlendState;

// =======================================================================================
//                                      RenderStateHelper
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Brief
///        
/// # RenderStateHelper
/// Detailed description.....
/// Created on: 4-1-2013 
///---------------------------------------------------------------------------------------

class RenderStateHelper
{
public:
	static void fillBlendStateList(ID3D11Device* p_device, 
		vector<ID3D11BlendState*>& p_blendStateList);

	static void fillRasterizerStateList(ID3D11Device* p_device, 
		vector<ID3D11RasterizerState*>& p_rasterizerStateList);
};