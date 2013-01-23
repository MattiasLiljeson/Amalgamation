#include "Utility.h"
#include "Quaternion.h"

float DotProduct(const AglVector3& v1, const AglVector3& v2)
{
	return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
}
AglVector3 CrossProduct(const AglVector3& v1, const AglVector3& v2)
{
	return AglVector3(v1.y*v2.z-v1.z*v2.y, v1.z*v2.x-v1.x*v2.z, v1.x*v2.y-v1.y*v2.x);
}
float Length(const AglVector3& v)
{
	return sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
}
float Length(const AglVector2& v)
{
	return sqrt(v[0]*v[0] + v[1]*v[1]);
}
float LengthSquared(const AglVector3& v)
{
	return v.x*v.x + v.y*v.y + v.z*v.z;
}
float LengthSquared(const AglVector2& v)
{
	return v[0]*v[0] + v[1]*v[1];
}
void Normalize(AglVector3& v)
{
	v = v / Length(v);
}
void Rotate(AglVector3& v, float Angle, AglVector3 Axis)
{
	Quaternion quat(v, 0);
	quat.Rotate(Angle, Axis);
	v = quat.GetUVector();
}
float RandomFloat()
{
	int limit = RAND_MAX;
	float number = (float)(rand() % limit+0)/RAND_MAX;
	return number;
}
float RandomFloat(float minValue, float maxValue)
{
	return RandomFloat()*(maxValue-minValue)+minValue;
}
AglVector3 RandomUnitVector3()
{
	AglVector3 vec = AglVector3(RandomFloat(-1, 1), RandomFloat(-1, 1), RandomFloat(-1, 1));
	Normalize(vec);
	return vec;
}

ShaderData* loadShaderBlob(string p_file)
{
	ifstream f;
	f.open(p_file, ios::in | ios::binary);
	 
	if (f.is_open())
	{
		SIZE_T size;
		f.read ((char*)&size, sizeof(SIZE_T));

		char* data = new char[size];

		f.read((char*)data, size);

		f.close();

		ShaderData* s = new ShaderData();
		s->data = (void*)data;
		s->size = size;
		return s;
	}
	return NULL;
}
void saveShaderBlob(string p_file, ID3DBlob* p_blob)
{
	ofstream f;
	f.open(p_file, ios::out | ios::binary);

	SIZE_T size = p_blob->GetBufferSize();
	f.write((char*)&size, sizeof(SIZE_T));
	void* data = p_blob->GetBufferPointer();
	f.write((char*)data, size);
	f.close();
}

string openfilename(char *filter, HWND owner) 
{
	OPENFILENAMEA ofn;
	char fileName[MAX_PATH] = "";
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = owner;
	ofn.lpstrFilter = filter;
	ofn.lpstrFile = fileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	ofn.lpstrDefExt = "";
	string fileNameStr;
	if ( GetOpenFileNameA(&ofn) )
	fileNameStr = fileName;
	return fileNameStr;
}
string savefilename(char *filter, HWND owner) 
{
	OPENFILENAMEA ofn;
	char fileName[MAX_PATH] = "";
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize   = sizeof(ofn);
	ofn.hwndOwner = owner;
	ofn.lpstrFile = fileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrFilter = "Agile (*.agl)\0*.agl\0";   
	ofn.lpstrTitle = "Save File As";
	ofn.Flags = OFN_HIDEREADONLY; 
	ofn.lpstrDefExt = "txt";
	string fileNameStr;
	if ( GetSaveFileNameA(&ofn) )
	fileNameStr = fileName;
	return fileNameStr;
}

void removePath(string& s)
{
	int ind = s.find_last_of('\\');
	if (ind == -1)
		ind = s.find_last_of('/');
	if (ind != -1)
		s = s.substr(ind+1, s.size() - ind);
}
string getPath(string s)
{
	int ind = s.find_last_of('\\');
	if (ind == -1)
		ind = s.find_last_of('/');
	return s.substr(0, ind+1);
}

ShaderData::~ShaderData()
{
	char* d = (char*)data;
	delete d;
}