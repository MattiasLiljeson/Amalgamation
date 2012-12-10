#include "TextureManager.h"

TextureManager* TextureManager::sInstance = NULL;

int TextureManager::NormMap = 0;
int TextureManager::DispMap = 0;

TextureManager::TextureManager()
{
	mDevice = NULL;
	mNoImage = NULL;
}
TextureManager::~TextureManager()
{
	for (int i = 0; i < mTextureData.size(); i++)
	{
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
	mNoImage = LoadTexture("NoImage.gif");
}
int TextureManager::LoadTexture(string pPath)
{
	/*for (int i = 0; i < pPath.size(); i++)
	{
		if (pPath[i] == '\\')
			pPath[i] = '/';
	}*/
	for (int i = 0; i < mTextureData.size(); i++)
	{
		if (pPath == mTextureData[i]->Path)
			return i;
	}
	
	std::wstring path2(pPath.length(), L' ');
	std::copy(pPath.begin(), pPath.end(), path2.begin());
	ID3D11ShaderResourceView* srv = loadTexture(mDevice, pPath.c_str());

	TextureData* d = new TextureData();
	d->SRV = srv;
	d->Path = pPath;
	mTextureData.push_back(d);
	return mTextureData.size()-1;
}
TextureData* TextureManager::GetTexture(int pIndex)
{
	if (pIndex < 0 || pIndex >= mTextureData.size())
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
	for (int i = 0; i < mTextureData.size(); i++)
	{
		if (mTextureData[i]->Path == pPath)
			return mTextureData[i];
	}
	return NULL;
}
int TextureManager::GetNoImage()
{
	return mNoImage;
}

ID3D11ShaderResourceView* TextureManager::loadTexture(ID3D11Device* p_device, 
											 const char* p_filePath)
{
	FREE_IMAGE_FORMAT imageFormat;
	FIBITMAP* image;

	imageFormat = FreeImage_GetFIFFromFilename(p_filePath);
	if( imageFormat != FIF_UNKNOWN )
		image = FreeImage_Load(imageFormat, p_filePath);
	else
		return NULL;

	FreeImage_FlipVertical(image);

	D3D11_SUBRESOURCE_DATA data;
	ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));

	BYTE* olddata = FreeImage_GetBits(image);
	BYTE* newdata = new BYTE[FreeImage_GetWidth(image) * FreeImage_GetHeight(image)*4];

	unsigned int bits = FreeImage_GetBPP(image);
	if (bits == 24)
	{
		unsigned int ind = 0;
		unsigned int counter = 0;
		for (unsigned int i = 0; i < FreeImage_GetWidth(image) * FreeImage_GetHeight(image) * 4; i++)
		{
			if (counter < 4)
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
	data.pSysMem = (void*)newdata;//FreeImage_GetBits(image);
	data.SysMemPitch = FreeImage_GetPitch(image);

	D3D11_TEXTURE2D_DESC texDesc;
	ZeroMemory(&texDesc,sizeof(D3D11_TEXTURE2D_DESC));
	texDesc.Width		= FreeImage_GetWidth(image);
	texDesc.Height		= FreeImage_GetHeight(image);
	texDesc.MipLevels	= 1;
	texDesc.ArraySize	= 1;
	texDesc.SampleDesc.Count	= 1;
	texDesc.SampleDesc.Quality	= 0;
	texDesc.Usage				= D3D11_USAGE_DEFAULT;
	texDesc.Format				= DXGI_FORMAT_B8G8R8A8_UNORM;
	texDesc.BindFlags			= D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags		= 0;
	texDesc.MiscFlags			= 0;

	ID3D11Texture2D* texture = NULL;
	HRESULT hr = p_device->CreateTexture2D( &texDesc, &data, &texture);

	if (!texture)
		return NULL;

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderDesc;
	ZeroMemory(&shaderDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	shaderDesc.Format = texDesc.Format;
	shaderDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderDesc.Texture2D.MipLevels = 1;
	shaderDesc.Texture2D.MostDetailedMip = 0;

	ID3D11ShaderResourceView* newShaderResurceView;
	hr = p_device->CreateShaderResourceView( texture, &shaderDesc, 
		&newShaderResurceView);


	/************************************************************************/
	/* Clean up the mess afterwards											*/
	/************************************************************************/
	FreeImage_Unload(image);
	texture->Release();

	return newShaderResurceView;
}