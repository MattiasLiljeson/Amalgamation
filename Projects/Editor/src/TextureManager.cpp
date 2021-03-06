#include "TextureManager.h"

TextureManager* TextureManager::sInstance = NULL;

int TextureManager::NormMap = 0;
int TextureManager::DispMap = 0;

TextureManager::TextureManager()
{
	mDevice = NULL;
}
TextureManager::~TextureManager()
{
	for (unsigned int i = 0; i < mTextureData.size(); i++)
	{
		if (mTextureData[i]->SRV)
			mTextureData[i]->SRV->Release();
		delete mTextureData[i];
	}
}

TextureManager* TextureManager::GetInstance()
{
	if (!sInstance)
		sInstance = new TextureManager();
	return sInstance;
}
void TextureManager::Release()
{
	if (sInstance)
		delete sInstance;
	sInstance = NULL;
}

void TextureManager::Init(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	mDevice = pDevice;
	mDeviceContext = pDeviceContext;
}
int TextureManager::LoadTexture(string pPath)
{
	/*for (int i = 0; i < pPath.size(); i++)
	{
		if (pPath[i] == '\\')
			pPath[i] = '/';
	}*/
	for (unsigned int i = 0; i < mTextureData.size(); i++)
	{
		if (pPath == mTextureData[i]->Path)
			return i;
	}
	
	std::wstring path2(pPath.length(), L' ');
	std::copy(pPath.begin(), pPath.end(), path2.begin());
	ID3D11ShaderResourceView* srv = loadTexture(mDevice, pPath.c_str());

	if (!srv)
	{
		string msg = "The file: '" + pPath + "' has been moved or deleted. Press OK to locate it. CANCEL to ignore.";
		int proceed = MessageBox(NULL, msg.c_str(), "Texture Warning", MB_OKCANCEL);
		if (proceed == IDOK)
		{
			pPath = openfilename();
			if (pPath != "")
			{
				return LoadTexture(pPath);
			}
			else
				return -1;
		}
		else
		{
			return -1;
		}
		
	}
	else
	{
		TextureData* d = new TextureData();
		d->SRV = srv;
		d->Path = pPath;
		mTextureData.push_back(d);
		return mTextureData.size()-1;
	}
}
TextureData* TextureManager::GetTexture(int pIndex)
{
	if (pIndex < 0 || pIndex >= (int)mTextureData.size())
		return NULL;
	return mTextureData[pIndex];
}
TextureData* TextureManager::GetTexture(string pPath)
{
	/*string path = pPath;
		int ind = path.find_last_of('\\');
		if (ind == -1)
			ind = path.find_last_of('/');
		path = path.substr(0, ind+1);	*/
	for (unsigned int i = 0; i < mTextureData.size(); i++)
	{
		if (mTextureData[i]->Path == pPath)
			return mTextureData[i];
	}
	return NULL;
}

ID3D11ShaderResourceView* TextureManager::loadTexture(ID3D11Device* p_device, 
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
	for(int i = 1; i< numLevels; i++)
	{
		ZeroMemory(&data[i], sizeof(D3D11_SUBRESOURCE_DATA));
		w = (unsigned int)max(1.0f, w*0.5f);
		h = (unsigned int)max(1.0f, h*0.5f);
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
		mDeviceContext->GenerateMips(newShaderResurceView);


	/************************************************************************/
	/* Clean up the mess afterwards											*/
	/************************************************************************/
	FreeImage_Unload(image);
	texture->Release();

	delete[] data;
	delete[] newdata;

	return newShaderResurceView;
}
void TextureManager::ReloadAll()
{
	for (unsigned int i = 0; i < mTextureData.size(); i++)
	{
		if (mTextureData[i]->SRV)
			mTextureData[i]->SRV->Release();
		mTextureData[i]->SRV = loadTexture(mDevice, mTextureData[i]->Path.c_str());
	}
}