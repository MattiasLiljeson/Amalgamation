#ifndef UTILITY_H
#define UTILITY_H

#include <iostream>
#include <vector>
#include "Timer.h"
#include <Windows.h>
#include <WindowsX.h>
#include <D3D11.h>
#include <d3dcompiler.h>
#include <string>
#include <fstream>
#include <ctime>
#include <sstream>
#include <AglVector3.h>
#include <Agl.h>
#include <AntTweakBar.h>

using namespace std;

typedef POINT       Point;
const double PI = 3.1415926536;

struct ShaderData
{
	void* data;
	SIZE_T size;
	~ShaderData();
};

float	DotProduct(const AglVector3& v1, const AglVector3& v2);
AglVector3 CrossProduct(const AglVector3& v1, const AglVector3& v2);
float	Length(const AglVector3& v);
float	Length(const AglVector2& v);
float	LengthSquared(const AglVector3& v);
float	LengthSquared(const AglVector2& v);
void	Normalize(AglVector3& v);
void	Rotate(AglVector3& v, float Angle, AglVector3 axis);
float	RandomFloat();
float	RandomFloat(float a, float b);
AglVector3 RandomUnitVector3();

template <class T>
string toString(T p_data)
{
	stringstream ss;
	ss << p_data;
	string s = ss.str();
	return s;
}

template <class T>
wstring toWString(T p_data)
{
	stringstream ss; 
	ss << p_data;
	string s = ss.str();

	wstring ws(s.length(), L' ');
	copy(s.begin(), s.end(), ws.begin());
	return ws;
}

ShaderData* loadShaderBlob(string p_file);
void saveShaderBlob(string p_file, ID3DBlob* p_blob);

string openfilename(char *filter = "All Files (*.*)\0*.*\0", HWND owner = NULL);
string savefilename(char *filter = "All Files (*.*)\0*.*\0", HWND owner = NULL);

void removePath(string& s);

#endif