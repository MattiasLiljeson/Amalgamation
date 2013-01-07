#include "TextureParser.h"


void TextureParser::init()
{
	FreeImage_Initialise();
}

ID3D11ShaderResourceView* TextureParser::loadTexture(ID3D11Device* p_device, 
											 const char* p_filePath)
{
	FREE_IMAGE_FORMAT imageFormat;
	FIBITMAP* image;
	bool succeededLoadingFile = false;
	ID3D11ShaderResourceView* newShaderResurceView;

	imageFormat = FreeImage_GetFIFFromFilename(p_filePath);
	if( imageFormat != FIF_UNKNOWN )
		image = FreeImage_Load(imageFormat, p_filePath);
	else
	{
		/************************************************************************/
		/* Made reverting back to a fallback texture will be enough? Instead	*/
		/* of throwing a exception.												*/
		/************************************************************************/
		succeededLoadingFile = false;
		throw FreeImageException("Unknown file format, cannot parse the file, " + 
		toString(p_filePath),__FILE__,__FUNCTION__,__LINE__);
	}
	if(succeededLoadingFile)
	{
		FreeImage_FlipVertical(image);

		newShaderResurceView = createTexture(
			p_device, FreeImage_GetBits(image), FreeImage_GetWidth(image),
			FreeImage_GetHeight(image), FreeImage_GetPitch(image),
			TextureParser::TEXTURE_TYPE::BGRA);

		/************************************************************************/
		/* Clean up the mess afterwards											*/
		/************************************************************************/
		FreeImage_Unload(image);
	}
	else
	{
		BYTE* data = generateFallbackTexture();
		newShaderResurceView = createTexture(p_device,data,10,10,32,
			TextureParser::TEXTURE_TYPE::RGBA);

		delete data;
	}
	return newShaderResurceView;
}

ID3D11ShaderResourceView* TextureParser::createTexture( ID3D11Device* p_device,
	const byte* p_source, int p_width, int p_height, int p_pitch, TEXTURE_TYPE p_type )
{
	D3D11_SUBRESOURCE_DATA data;
	ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
	data.pSysMem = (void*)p_source;
	data.SysMemPitch = p_pitch;

	D3D11_TEXTURE2D_DESC texDesc;
	ZeroMemory(&texDesc,sizeof(D3D11_TEXTURE2D_DESC));
	texDesc.Width				= p_width;
	texDesc.Height				= p_height;
	texDesc.MipLevels			= 1;
	texDesc.ArraySize			= 1;
	texDesc.SampleDesc.Count	= 1;
	texDesc.SampleDesc.Quality	= 0;
	texDesc.Usage				= D3D11_USAGE_DEFAULT;
	texDesc.CPUAccessFlags		= 0;
	texDesc.MiscFlags			= 0;
	texDesc.BindFlags			= D3D11_BIND_SHADER_RESOURCE;

	switch( p_type )
	{
	case RGBA:
		texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		break;		   

	case BGRA:		   
		texDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		break;		   

	default:		   
		texDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		break;
	}

	ID3D11Texture2D* texture = NULL;
	HRESULT hr = p_device->CreateTexture2D( &texDesc, &data, &texture);

	if (FAILED(hr))
		throw D3DException(hr, __FILE__,__FUNCTION__,__LINE__);

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderDesc;
	ZeroMemory(&shaderDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	shaderDesc.Format = texDesc.Format;
	shaderDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderDesc.Texture2D.MipLevels = 1;
	shaderDesc.Texture2D.MostDetailedMip = 0;

	ID3D11ShaderResourceView* newShaderResurceView;
	hr = p_device->CreateShaderResourceView( texture, &shaderDesc, 
		&newShaderResurceView);

	if (FAILED(hr))
		throw D3DException(hr, __FILE__,__FUNCTION__,__LINE__);

	texture->Release();

	return newShaderResurceView;
}

BYTE* TextureParser::generateFallbackTexture()
{
	int dimension = 10;
	int size = dimension*dimension;
	int bitDepth = 4;
	BYTE* textureData = new BYTE[size*bitDepth];

	unsigned int counterX = 0;
	unsigned int counterY = 0;
	bool pink = true;
	for (int i = 0; i < size*bitDepth;i+=4)
	{
		if(counterX == dimension)
		{
			counterY++;
			counterX = 0;
		}

		//Top LEFT
		if(counterX < 5 && counterY < 5)
			pink = true;
		//Top RIGHT
		else if(counterX > 4 && counterY < 5)
			pink = false;
		//Bot LEFT
		else if(counterX < 5 && counterY > 4)
			pink = false;
		//Bot RIGHT
		else 
			pink = true;

		if(pink)
		{
			textureData[i]		= 255;	//RED
			textureData[i+1]	= 0;	//BLUE
			textureData[i+2]	= 255;	//GREEN
			textureData[i+3]	= 255;	//ALPHA
		}
		else
		{
			textureData[i]		= 0;
			textureData[i+1]	= 0;
			textureData[i+2]	= 0;
			textureData[i+3]	= 255;
		}
		counterX++;
	}	

	return textureData;
}
