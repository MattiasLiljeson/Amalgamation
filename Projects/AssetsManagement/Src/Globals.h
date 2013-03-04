#pragma once
#include <string>

using namespace std;

// =======================================================================================
//                                      Globals
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Holds all the global defined variables
///        
/// # Globals
/// Detailed description.....
/// Created on: 14-12-2012 
///---------------------------------------------------------------------------------------

static const string& SOUNDROOTPATH = "Assets/Sound/";
static const string& SOUNDEFFECTPATH = "Assets/Sound/SoundEffect/";
static const string& MUSICPATH = "Assets/Sound/Music/";
static const string& MODELPATH = "Assets/Models/";
static const string& TEXTUREPATH = "Assets/Textures/";

/************************************************************************/
/* LIBROCKET PATHS!														*/
/************************************************************************/

static const string& GUI_CURSOR_PATH	= "GUI/Cursor/";
static const string& GUI_HUD_PATH		= "GUI/Hud/";
static const string& GUI_MENU_PATH		= "GUI/Menu/";
static const string& GUI_FONT_PATH		= "Assets/GUI/Fonts/";
static const string& GUI_TEXTURE_PATH	= "Assets/GUI/Texture/";

/* TEST PATHS! */

static const string& TESTSOUNDEFFECTPATH = "Assets/Sound/SoundEffect/Test/";
static const string& TESTMUSICPATH = "Assets/Sound/Music/Test/";

static const string& TESTMODELPATH = "Assets/Models/Test/";
static const string& TESTTEXTUREPATH = "Assets/Textures/Test/";

static const string& SETTINGSPATH = "Settings/"; 
static const string& ASSEMBLAGESPATH = "Assemblages/";
static const string& LEVELPIECESPATH = ASSEMBLAGESPATH + "Level Pieces/";
/************************************************************************/
/* MISC														*/
/************************************************************************/
static const int MAXPLAYERS = 8;
static const int MAXSHADOWS = 2;

/************************************************************************/
/* HUD																	*/
/************************************************************************/
static const char* TIMERELEMENT = "time";
static const char* SCOREELEMENT = "score";
static const char* MAPPINGELEMENT = "mapping";