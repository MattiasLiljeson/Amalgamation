#include "StandardShader.h"
#include "Camera.h"
#include "TextureManager.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "HullShader.h"
#include "DomainShader.h"
#include "Scene.h"
#include "DX11Application.h"
#include "Globals.h"

//http://developer.download.nvidia.com/whitepapers/2010/PN-AEN-Triangles-Whitepaper.pdf

StandardShader::StandardShader(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, VertexShader* pVS, PixelShader* pPS, HullShader* pHS, DomainShader* pDS)
{
	mDevice			= pDevice;
	mDeviceContext	= pDeviceContext;

	mVS = pVS;
	mPS = pPS;
	mHS = pHS;
	mDS = pDS;

	D3D11_INPUT_ELEMENT_DESC PositionNormalTexCoord[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"BITANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 44, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	mDevice->CreateInputLayout(PositionNormalTexCoord, 5, mVS->GetCompiledData(), mVS->GetCompiledSize(), &mIL);


	D3D11_BUFFER_DESC BufferDesc;
	BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	BufferDesc.ByteWidth = sizeof(StandardShaderBuffer);
	BufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	BufferDesc.MiscFlags = 0;
	BufferDesc.StructureByteStride = 0;
	pDevice->CreateBuffer(&BufferDesc, NULL, &mBuffer);

	BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	BufferDesc.ByteWidth = sizeof(MaterialBuffer);
	BufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	BufferDesc.MiscFlags = 0;
	BufferDesc.StructureByteStride = 0;
	pDevice->CreateBuffer(&BufferDesc, NULL, &mMaterialBuffer);

	BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	BufferDesc.ByteWidth = sizeof(DomainBuffer);
	BufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	BufferDesc.MiscFlags = 0;
	BufferDesc.StructureByteStride = 0;
	pDevice->CreateBuffer(&BufferDesc, NULL, &mDomainBuffer);

	BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	BufferDesc.ByteWidth = sizeof(HullBuffer);
	BufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	BufferDesc.MiscFlags = 0;
	BufferDesc.StructureByteStride = 0;
	pDevice->CreateBuffer(&BufferDesc, NULL, &mHullBuffer);


	D3D11_SAMPLER_DESC samplerDesc;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	pDevice->CreateSamplerState(&samplerDesc, &mSampler);
}
StandardShader::~StandardShader()
{
	mIL->Release();
	mBuffer->Release();

}
ID3D11VertexShader* StandardShader::GetVertexShader()
{
	return mVS->GetShader();
}
ID3D11PixelShader*	StandardShader::GetPixelShader()
{
	return mPS->GetShader();
}
ID3D11HullShader*	StandardShader::GetHullShader()
{
	return mHS->GetShader();
}
ID3D11DomainShader*	StandardShader::GetDomainShader()
{
	return mDS->GetShader();
}
ID3D11InputLayout* StandardShader::GetInputLayout()
{
	return mIL;
}
void StandardShader::SetBuffer(AglMatrix pWorld, AglMatrix pView, AglMatrix pProjection, float pScale, AglMaterial pMaterial)
{
	pWorld = AglMatrix::transpose(pWorld);
	pView = AglMatrix::transpose(pView);
	pProjection = AglMatrix::transpose(pProjection);

	//Vertex shader cbuffer
	D3D11_MAPPED_SUBRESOURCE resource;
	mDeviceContext->Map(mBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);

	StandardShaderBuffer* buffer;
	buffer = (StandardShaderBuffer*)resource.pData;
	buffer->World = pWorld;
	buffer->View = pView;
	buffer->Projection = pProjection;
	buffer->Scale = pScale;
	buffer->TextureScale = pMaterial.textureScale;

	mDeviceContext->Unmap(mBuffer, 0);

	//Domain shader cbuffer
	mDeviceContext->Map(mDomainBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);

	static float displ = 0.0f;
	displ += DX11Application::dt*0.01f;
	DomainBuffer* dbuffer;
	dbuffer = (DomainBuffer*)resource.pData;
	dbuffer->World = pWorld;
	dbuffer->View = pView;
	dbuffer->Projection = pProjection;
	dbuffer->Scale = pScale;
	dbuffer->Displacement = pMaterial.displacement;
	dbuffer->UseAlpha = (pMaterial.normalTextureNameIndex == pMaterial.displacementTextureNameIndex);

	mDeviceContext->Unmap(mDomainBuffer, 0);

	//Hull shader cbuffer
	mDeviceContext->Map(mHullBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	static float subd = 0.0f;
	subd += DX11Application::dt;
	if (subd > 12)
		subd = 1;
	HullBuffer* hbuffer;
	hbuffer = (HullBuffer*)resource.pData;
	hbuffer->Amount[0] = pMaterial.tesselationFactor.x;
	hbuffer->Amount[1] = pMaterial.tesselationFactor.y;
	hbuffer->Amount[2] = pMaterial.tesselationFactor.z;
	hbuffer->Amount[3] = pMaterial.tesselationFactor.w;
	mDeviceContext->Unmap(mHullBuffer, 0);


	unsigned int bufferNumber = 0;
	mDeviceContext->VSSetConstantBuffers(bufferNumber, 1, &mBuffer);
	mDeviceContext->DSSetConstantBuffers(bufferNumber, 1, &mDomainBuffer);
	mDeviceContext->HSSetConstantBuffers(bufferNumber, 1, &mHullBuffer);

	//Displacement map
	//TextureData* disp = TextureManager::GetInstance()->GetTexture(TextureManager::DispMap);
	//mDeviceContext->DSSetShaderResources(0, 1, &disp->SRV);

	//Copy the material buffer to the gpu
	mDeviceContext->Map(mMaterialBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);

	MaterialBuffer* matbuffer;
	matbuffer = (MaterialBuffer*)resource.pData;
	matbuffer->AmbientOpacity = AglVector4(pMaterial.ambient, pMaterial.opacity);
	matbuffer->DiffuseReflectivity = AglVector4(pMaterial.diffuse, pMaterial.reflectivity);
	matbuffer->SpecularShininess = AglVector4(pMaterial.specular, pMaterial.shininess);
	matbuffer->EmissiveDiffuseMapped = AglVector4(pMaterial.emissive, float(pMaterial.diffuseTextureNameIndex >= 0 && DIFFUSEON));
	matbuffer->EyePositionSpecularMapped = AglVector4(Camera::GetInstance()->Position().x, Camera::GetInstance()->Position().y, Camera::GetInstance()->Position().z, float(pMaterial.specularTextureNameIndex >= 0  && SPECULARON));
	matbuffer->Flags = AglVector4((float)(pMaterial.glowTextureNameIndex >= 0 && GLOWON), (float)(pMaterial.normalTextureNameIndex >= 0 && NORMALON), (float)(pMaterial.gradientDataIndex >= 0 && pMaterial.gradientTextureNameIndex >= 0), 0);
	if (pMaterial.gradientDataIndex >= 0)
	{
		AglGradient* g = Scene::GetInstance()->GetGradient(pMaterial.gradientDataIndex);

		vector<AglGradientMaterial*> layers = g->getLayers();
		for (unsigned int i = 0; i < layers.size(); i++)
			matbuffer->gradientColors[i] = layers[i]->color;
		matbuffer->Flags.w = (float)layers.size();
	}
	
	mDeviceContext->Unmap(mMaterialBuffer, 0);

	mDeviceContext->PSSetConstantBuffers(bufferNumber, 1, &mMaterialBuffer);

	if (pMaterial.diffuseTextureNameIndex >= 0)
	{
		string s = Scene::GetInstance()->GetName(pMaterial.diffuseTextureNameIndex);
		TextureData* data = TextureManager::GetInstance()->GetTexture(s);
		if (data)
			mDeviceContext->PSSetShaderResources(0, 1, &data->SRV);
	}
	if (pMaterial.specularTextureNameIndex >= 0)
	{
		string s = Scene::GetInstance()->GetName(pMaterial.specularTextureNameIndex);
		TextureData* data = TextureManager::GetInstance()->GetTexture(s);
		if (data)
			mDeviceContext->PSSetShaderResources(1, 1, &data->SRV);
	}
	if (pMaterial.glowTextureNameIndex >= 0)
	{
		string s = Scene::GetInstance()->GetName(pMaterial.glowTextureNameIndex);
		TextureData* data = TextureManager::GetInstance()->GetTexture(s);
		if (data)
			mDeviceContext->PSSetShaderResources(2, 1, &data->SRV);
	}
	if (pMaterial.normalTextureNameIndex >= 0)
	{
		string s = Scene::GetInstance()->GetName(pMaterial.normalTextureNameIndex);
		TextureData* data = TextureManager::GetInstance()->GetTexture(s);
		if (data)
		{
			mDeviceContext->PSSetShaderResources(3, 1, &data->SRV);
		}
	}
	if (pMaterial.displacementTextureNameIndex >= 0)
	{
		string s = Scene::GetInstance()->GetName(pMaterial.displacementTextureNameIndex);
		TextureData* data = TextureManager::GetInstance()->GetTexture(s);
		if (data)
			mDeviceContext->DSSetShaderResources(0, 1, &data->SRV);
	}
	if (pMaterial.gradientTextureNameIndex >= 0)
	{
		string s = Scene::GetInstance()->GetName(pMaterial.gradientTextureNameIndex);
		TextureData* data = TextureManager::GetInstance()->GetTexture(s);
		if (data)
			mDeviceContext->PSSetShaderResources(4, 1, &data->SRV);
	}

	mDeviceContext->PSSetSamplers(0, 1, &mSampler);
	mDeviceContext->DSSetSamplers(0, 1, &mSampler);
}
void StandardShader::SetBuffer(AglMatrix pWorld, AglMatrix pView, AglMatrix pProjection, float pScale, Gradient* pGradient)
{
}