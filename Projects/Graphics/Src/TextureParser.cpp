#include "TextureParser.h"
#include <DebugUtil.h>

void TextureParser::init()
{
	FreeImage_Initialise();
}

ID3D11ShaderResourceView* TextureParser::loadTexture(ID3D11Device* p_device, 
											 const char* p_filePath)
{
	FREE_IMAGE_FORMAT imageFormat;
	FIBITMAP* image;
	bool succeededLoadingFile = true;
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
		DEBUGWARNING(((string("Unknown texture file format, cannot parse the file, reverting to fallback texture. ") + 
					  toString(p_filePath)).c_str()));
	}
	/************************************************************************/
	/* If the width of the image is equal to zero, then the file wasn't		*/
	/* found																*/
	/************************************************************************/
	if (succeededLoadingFile && FreeImage_GetWidth(image)==0)
	{
		DEBUGWARNING(((string("Texture file not found, reverting to fallback texture. ") + 
			toString(p_filePath)).c_str()));
		succeededLoadingFile = false;
	}
	if(succeededLoadingFile)
	{
		FreeImage_FlipVertical(image);

		newShaderResurceView = createTexture(
			p_device, FreeImage_GetBits(image), FreeImage_GetWidth(image),
			FreeImage_GetHeight(image), FreeImage_GetPitch(image), FreeImage_GetBPP(image),
			TextureParser::TEXTURE_TYPE::BGRA);

		/************************************************************************/
		/* Clean up the mess afterwards											*/
		/************************************************************************/
		FreeImage_Unload(image);
	}
	else
	{
		BYTE* data = generateFallbackTexture();
		newShaderResurceView = createTexture(p_device,data,10,10,128,32,
			TextureParser::TEXTURE_TYPE::RGBA);

		delete data;
	}
	return newShaderResurceView;
}

ID3D11ShaderResourceView* TextureParser::createTexture( ID3D11Device* p_device,
	const byte* p_source, int p_width, int p_height, int p_pitch, int p_bitLevel, 
	TEXTURE_TYPE p_type )
{
	byte* newData = NULL;

	if(p_bitLevel == 24){
		newData = new byte[p_width*p_height*4];
		unsigned int ind = 0;
		unsigned int counter = 0;
		for (unsigned int i = 0; i < p_width * p_height*4;i++){
			if(counter < 3){
				newData[i] = p_source[ind++];
				counter++;
			}
			else{
				newData[i] = 255;
				counter = 0;
			}
		}
	}

	D3D11_SUBRESOURCE_DATA data;
	ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
	if(newData){
		data.pSysMem = (void*)newData;
	}
	else{
		data.pSysMem = (void*)p_source;
	}
	data.SysMemPitch = p_pitch;
	data.SysMemSlicePitch = 0;

	D3D11_TEXTURE2D_DESC texDesc;
	ZeroMemory(&texDesc,sizeof(D3D11_TEXTURE2D_DESC));
	texDesc.Width				= p_width;
	texDesc.Height				= p_height;
	texDesc.MipLevels			= 1;
	texDesc.ArraySize			= 1;
	texDesc.SampleDesc.Count	= 1;
	texDesc.SampleDesc.Quality	= 0;
	texDesc.Usage				= D3D11_USAGE_DEFAULT;
	texDesc.BindFlags			= D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags		= 0;
	texDesc.MiscFlags			= 0;

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
	delete newData;

	return newShaderResurceView;
}

BYTE* TextureParser::generateFallbackTexture()
{
	int dimension = 10;
	int size = dimension*dimension;
	int bitDepth = 4;
	BYTE* textureData = new BYTE[size*bitDepth];
	bool pink = true;

	/************************************************************************/
	/* DOSEN'T GENERATE A CHECKSQUARED TEXTURE, but cannot find the issue.  */
	/* -Robin																*/
	/************************************************************************/
	for (int y = 0; y < dimension; y++)
	{
		for (int x = 0; x < dimension; x++)
		{
			if(x == 4)
			{
				if(pink)
					pink = false;
				else
					pink = true;
			}
			if(pink)
			{
				textureData[dimension*y*bitDepth + x*bitDepth]		= 255;	//RED
				textureData[dimension*y*bitDepth + x*bitDepth+1]	= 0;	//BLUE
				textureData[dimension*y*bitDepth + x*bitDepth+2]	= 255;	//GREEN
				textureData[dimension*y*bitDepth + x*bitDepth+3]	= 255;	//ALPHA
			}
			else
			{
				textureData[dimension*y*bitDepth + x*bitDepth]		= 0;
				textureData[dimension*y*bitDepth + x*bitDepth+1]	= 0;
				textureData[dimension*y*bitDepth + x*bitDepth+2]	= 0;
				textureData[dimension*y*bitDepth + x*bitDepth+3]	= 255;
			}
		}

		if(y < 5)
			pink = true;
		else
			pink = false;
	}
	
	return textureData;
}
