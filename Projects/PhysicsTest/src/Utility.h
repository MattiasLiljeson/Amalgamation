#ifndef UTILITY_H
#define UTILITY_H

#include <iostream>
#include <vector>
#include "Timer.h"
#include <Windows.h>
#include <WindowsX.h>
#include <D3D11.h>
#include <string>
#include <fstream>
#include <ctime>
#include <sstream>
#include <AglMatrix.h>
#include <AglVector3.h>
#include <d3dcompiler.h>
#include <FreeImage.h>

using namespace std;

typedef POINT       Point;
const double PI = 3.141593;

void	Normalize(AglVector3& v);
void	NormalizeFast(AglVector3& v);
void	Rotate(AglVector3& v, float Angle, AglVector3 axis);
float	RandomFloat();
float	RandomFloat(float a, float b);
AglVector3 RandomUnitVector3();

AglVector4	Vec4MatrixMult(AglVector4& vec, AglMatrix mat);
AglVector3	Vec3Transform(AglVector3& vec, AglMatrix mat);
AglVector3	Vec3TransformNormal(AglVector3& vec, AglMatrix mat);

template <class T>
string ToString(T p_data)
{
	stringstream ss;
	ss << p_data;
	string s = ss.str();
	return s;
}
float InverseSqrt(float pNumber);

float Det(AglVector3 pV1, AglVector3 pV2, AglVector3 pV3);

ID3D11ShaderResourceView* loadTexture(ID3D11Device* p_device, ID3D11DeviceContext* p_deviceContext,
									  const char* p_filePath);

string openfilename(char *filter = "All Files (*.*)\0*.*\0", HWND owner = NULL);



#endif