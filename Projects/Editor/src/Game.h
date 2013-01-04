#ifndef GAME_H
#define GAME_H

#include "DX11Application.h"
#include "Timer.h"
#include "Scene.h"
#include "AGLLoader.h"
#include "Camera.h"
#include "CameraController.h"
#include "ShaderManager.h"
#include "TextureManager.h"
#include "SceneDialog.h"

class Game: public DX11Application
{
private:
	CameraController* m_cameraController;
public:
	Game(HINSTANCE pInstanceHandle);
	bool Initialize();
	bool Cleanup();
	bool Update(float pElapsedTime);
	bool Draw(float pElapsedTime);
};

#endif