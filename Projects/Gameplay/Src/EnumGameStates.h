#pragma once


enum class GameStates
{
	MENU, LOBBY, LOADING, INGAME, PAUSE, NUMSTATES
};


enum class EnumGameDelta
{
	NOTCHANGED, ENTEREDTHISFRAME
};


enum class ServerStates
{
	LOBBY, LOADING, INGAME, RESULT, NUMSTATES
};
