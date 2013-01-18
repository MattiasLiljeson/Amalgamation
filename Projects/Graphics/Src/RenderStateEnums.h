#pragma once

// =======================================================================================
//                                      RenderStateEnums
// =======================================================================================


// Enumerators used by both GraphicsWrapper and DeferredRenderer to set various
// render properties such as blending- and rasterization settings.


struct RasterizerState
{
	enum Mode
	{
		DEFAULT=0,
		FILLED_CW,				// Filled, backface-cull, clockwise
		FILLED_CCW,				// Filled, backface-cull, counter-clockwise
		FILLED_CW_FRONTCULL,	// Filled, frontface-cull, counter-clockwise
		FILLED_CW_SCISSOR,		// Filled, backface-cull, clockwise, scissor culling
		FILLED_CCW_SCISSOR,		// Filled, backface-cull, counter-clockwise, scissor culling
		FILLED_NOCULL,			// Filled, no cull
		WIREFRAME,				// Wireframe, backface-cull
		WIREFRAME_FRONTCULL,// Wireframe, frontface-cull
		WIREFRAME_NOCULL,		// Wireframe, no cull
		NUMBER_OF_MODES
	};
};


struct BlendState
{
	enum Mode
	{
		DEFAULT=0,
		NORMAL,
		ALPHA,
		MULTIPLY,
		ADDITIVE,
		NUMBER_OF_MODES
	};
};