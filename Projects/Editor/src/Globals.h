#ifndef GLOBALS_H
#define GLOBALS_H

extern bool DIFFUSEON;
extern bool SPECULARON;
extern bool GLOWON;
extern bool NORMALON;
extern float MOUSEWHEELDELTA;
enum CoordinateSystem
{
	DX, GL, BLENDER
};

extern CoordinateSystem COSYSTEM;

#endif