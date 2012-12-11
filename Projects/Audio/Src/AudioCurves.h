#pragma once
#include <xaudio2.h>
#include <x3daudio.h>

// =======================================================================================
//                                      AudioCurves
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Contains sound curves for XAudio2
///        
/// # AudioCurves
/// Please see documentation for each aduio curve
/// Created on: 7-12-2012 
///---------------------------------------------------------------------------------------

///---------------------------------------------------------------------------------------
/// Listener_DirectionalCone defines the cone for the listener, if something is within
/// this cone it is considered to be in front of the player.
/// Emitters behind the listener are defined here to be more attenuated, have a lower LPF 
/// cutoff frequency, yet have a slightly higher reverb send level.
///---------------------------------------------------------------------------------------
static const X3DAUDIO_CONE Listener_DirectionalCone = 
{ X3DAUDIO_PI*5.0f/6.0f, X3DAUDIO_PI*11.0f/6.0f, 1.0f, 0.75f, 0.0f, 0.25f, 0.708f, 1.0f };

///---------------------------------------------------------------------------------------
/// Emitter_LFE_CurvePoints, specifies the LFE level distance curve such that it rolls off
/// much sooner than all non-LFE channels, making use of the subwoofer more dramatic.
///---------------------------------------------------------------------------------------
static const X3DAUDIO_DISTANCE_CURVE_POINT Emitter_LFE_CurvePoints[3] = 
{ 0.0f, 1.0f, 0.25f, 0.0f, 1.0f, 0.0f };
static const X3DAUDIO_DISTANCE_CURVE       Emitter_LFE_Curve          = 
{ (X3DAUDIO_DISTANCE_CURVE_POINT*)&Emitter_LFE_CurvePoints[0], 3 };

///---------------------------------------------------------------------------------------
/// Emitter_Reverb_CurvePoints, specifies reverb send level distance curve such that reverb
/// send increases slightly with distance before rolling off to silence. With the direct 
/// channels being increasingly attenuated with distance, this has the effect of increasing
/// the reverb-to-direct sound ratio, reinforcing the perception of distance. 
///---------------------------------------------------------------------------------------
static const X3DAUDIO_DISTANCE_CURVE_POINT Emitter_Reverb_CurvePoints[3] = 
{ 0.0f, 0.5f, 0.75f, 1.0f, 1.0f, 0.0f };
static const X3DAUDIO_DISTANCE_CURVE       Emitter_Reverb_Curve          = 
{ (X3DAUDIO_DISTANCE_CURVE_POINT*)&Emitter_Reverb_CurvePoints[0], 3 };