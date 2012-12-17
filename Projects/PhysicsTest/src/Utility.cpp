#include "Utility.h"
#include "Quaternion.h"

void Normalize(AglVector3& v)
{
	v = v / AglVector3::length(v);
}
void NormalizeFast(AglVector3& v)
{
	v = v * InverseSqrt(AglVector3::lengthSquared(v));
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

AglVector4	Vec4MatrixMult(AglVector4& vec, AglMatrix mat)
{
	vec = AglVector4(mat[0] *vec.x + mat[4] *vec.y + mat[8] *vec.z + mat[12] *vec.w,
						mat[1] *vec.x + mat[5] *vec.y + mat[9] *vec.z + mat[13] *vec.w,
							mat[2] *vec.x + mat[6] *vec.y + mat[10] *vec.z + mat[14] *vec.w,
								mat[3] *vec.x + mat[7] *vec.y + mat[11] *vec.z + mat[15] *vec.w);
	return vec;
}
AglVector3	Vec3Transform(AglVector3& vec, AglMatrix mat)
{
	AglVector4 v4(vec[0], vec[1], vec[2], 1);
	Vec4MatrixMult(v4, mat);
	vec = AglVector3(v4.x, v4.y, v4.z);
	return vec;
}
AglVector3	Vec3TransformNormal(AglVector3& vec, AglMatrix mat)
{
	AglVector4 v4(vec[0], vec[1], vec[2], 0);
	Vec4MatrixMult(v4, mat);
	vec = AglVector3(v4.x, v4.y, v4.z);
	return vec;
}


float InverseSqrt(float pNumber)
{
        long i;
        float x2, y;
        const float threehalfs = 1.5f;
 
        x2 = pNumber * 0.5F;
        y  = pNumber;
        i  = * ( long * ) &y;                       // evil floating point bit level hacking
        i  = 0x5f3759df - ( i >> 1 );               // what the fuck?
        y  = * ( float * ) &i;
        y  = y * ( threehalfs - ( x2 * y * y ) );   // 1st iteration
//      y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed
 
        return y;
}

float Det(AglVector3 pV1, AglVector3 pV2, AglVector3 pV3)
{
	float r1 = (pV1[0]*pV2[1]*pV3[2]) + (pV2[0]*pV3[1]*pV1[2]) + (pV3[0]*pV1[1]*pV2[2]);
	float r2 = (pV1[0]*pV3[1]*pV2[2]) + (pV2[0]*pV1[1]*pV3[2]) + (pV3[0]*pV2[1]*pV1[2]);
	return r1 - r2;
}

ID3D11ShaderResourceView* loadTexture(ID3D11Device* p_device, ID3D11DeviceContext* p_deviceContext,
									  const char* p_filePath)
{
	//Not optimal! Loaded textures should really be immutable.

	//Does not work with GenerateMipmaps which has to be created by hand in that case.

	//Could use CopySubresourceRegion to copy the result of a mipmapped texture to a immutable one
	//See http://msdn.microsoft.com/en-us/library/windows/desktop/ff476259(v=vs.85).aspx

	FREE_IMAGE_FORMAT imageFormat;
	FIBITMAP* image;

	imageFormat = FreeImage_GetFIFFromFilename(p_filePath);
	if( imageFormat != FIF_UNKNOWN )
		image = FreeImage_Load(imageFormat, p_filePath);
	else
		return NULL;

	FreeImage_FlipVertical(image);

	unsigned int width = FreeImage_GetWidth(image);
	unsigned int height = FreeImage_GetHeight(image);
	int numLevels = 1;
	while((width > 1) || (height > 1))
	{
		width = max(width / 2, 1);
		height = max(height / 2, 1);
		++numLevels;
	}

	D3D11_SUBRESOURCE_DATA* data = new D3D11_SUBRESOURCE_DATA[numLevels];
	ZeroMemory(&data[0], sizeof(D3D11_SUBRESOURCE_DATA));

	BYTE* olddata = FreeImage_GetBits(image);
	BYTE* newdata = new BYTE[FreeImage_GetWidth(image) * FreeImage_GetHeight(image)*4];

	unsigned int bits = FreeImage_GetBPP(image);
	if (bits == 24)
	{
		unsigned int ind = 0;
		unsigned int counter = 0;
		for (unsigned int i = 0; i < FreeImage_GetWidth(image) * FreeImage_GetHeight(image) * 4; i++)
		{
			if (counter < 3)
			{
				newdata[i] = olddata[ind++];
				counter++;
			}
			else
			{
				newdata[i] = 255;
				counter = 0;
			}
		}
	}
	else
	{
		for (unsigned int i = 0; i < FreeImage_GetWidth(image) * FreeImage_GetHeight(image) * 4; i++)
		{
			newdata[i] = olddata[i];
		}
	}
	data[0].pSysMem = (void*)newdata;//FreeImage_GetBits(image);
	data[0].SysMemPitch = FreeImage_GetWidth(image)*4;
	data[0].SysMemSlicePitch = 0;
	unsigned int w = FreeImage_GetWidth(image);
	unsigned int h = FreeImage_GetHeight(image);
	vector<unsigned char*> levelData;
	for(unsigned int i = 1; i< numLevels; i++)
	{
		ZeroMemory(&data[i], sizeof(D3D11_SUBRESOURCE_DATA));
		w = max(1, w*0.5f);
		h = max(1, h*0.5f);
		levelData.push_back(new unsigned char[4*w*h]);
		data[i].pSysMem = levelData.back();
		data[i].SysMemPitch = 4 * w;
		data[i].SysMemSlicePitch = 0;
	} 

	D3D11_TEXTURE2D_DESC texDesc;
	ZeroMemory(&texDesc,sizeof(D3D11_TEXTURE2D_DESC));
	texDesc.Width		= FreeImage_GetWidth(image);
	texDesc.Height		= FreeImage_GetHeight(image);
	texDesc.MipLevels	= numLevels;
	texDesc.ArraySize	= 1;
	texDesc.SampleDesc.Count	= 1;
	texDesc.SampleDesc.Quality	= 0;
	texDesc.Usage				= D3D11_USAGE_DEFAULT;
	texDesc.Format				= DXGI_FORMAT_B8G8R8A8_UNORM;
	texDesc.BindFlags			= D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	texDesc.CPUAccessFlags		= 0;
	texDesc.MiscFlags			= D3D11_RESOURCE_MISC_GENERATE_MIPS;

	ID3D11Texture2D* texture = NULL;
	HRESULT hr = p_device->CreateTexture2D( &texDesc, data, &texture);

	for (unsigned int i = 0; i < levelData.size(); i++)
		delete[] levelData[i];

	if (!texture)
	{
		delete[] data;
		delete[] newdata;
		return NULL;
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderDesc;
	ZeroMemory(&shaderDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	shaderDesc.Format = texDesc.Format;
	shaderDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderDesc.Texture2D.MipLevels = numLevels;
	shaderDesc.Texture2D.MostDetailedMip = 0;

	ID3D11ShaderResourceView* newShaderResurceView;
	hr = p_device->CreateShaderResourceView( texture, &shaderDesc, 
											&newShaderResurceView);

	bool mips = true;
	if (mips)
		p_deviceContext->GenerateMips(newShaderResurceView);


	/************************************************************************/
	/* Clean up the mess afterwards											*/
	/************************************************************************/
	FreeImage_Unload(image);
	texture->Release();

	delete[] data;
	delete[] newdata;

	return newShaderResurceView;
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