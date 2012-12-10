#include "SkeletonMeshShader.h"
#include "AGLLoader.h"
#include "Camera.h"
#include "TextureManager.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "Scene.h"

SkeletonMeshShader::SkeletonMeshShader(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, VertexShader* pVS, PixelShader* pPS)
{
	mDevice			= pDevice;
	mDeviceContext	= pDeviceContext;

	mVS = pVS;
	mPS = pPS;

	D3D11_INPUT_ELEMENT_DESC PositionNormalTexCoordWeights[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"BITANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 44, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"AWEIGHTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"BWEIGHTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"AINDICES", 0, DXGI_FORMAT_R32G32B32A32_UINT, 1, 32, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"BINDICES", 0, DXGI_FORMAT_R32G32B32A32_UINT, 1, 48, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	mDevice->CreateInputLayout(PositionNormalTexCoordWeights, 9, mVS->GetCompiledData(), mVS->GetCompiledSize(), &mIL);

	D3D11_BUFFER_DESC BufferDesc;
	BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	BufferDesc.ByteWidth = sizeof(SkeletonMeshShaderBuffer);
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
SkeletonMeshShader::~SkeletonMeshShader()
{
	mIL->Release();
	mBuffer->Release();
	mMaterialBuffer->Release();

}
ID3D11VertexShader* SkeletonMeshShader::GetVertexShader()
{
	return mVS->GetShader();
}
ID3D11PixelShader*	SkeletonMeshShader::GetPixelShader()
{
	return mPS->GetShader();
}
ID3D11InputLayout* SkeletonMeshShader::GetInputLayout()
{
	return mIL;
}
void SkeletonMeshShader::SetBuffer(AglMatrix pWorld, AglMatrix pView, AglMatrix pProjection, float pScale, AglSkeleton* pSkeleton, AglMaterial pMaterial)
{
	pWorld = AglMatrix::transpose(pWorld);
	pView = AglMatrix::transpose(pView);
	pProjection = AglMatrix::transpose(pProjection);

	//Copy the matrix palette buffer to the gpu
	D3D11_MAPPED_SUBRESOURCE resource;
	mDeviceContext->Map(mBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);

	SkeletonMeshShaderBuffer* buffer;
	buffer = (SkeletonMeshShaderBuffer*)resource.pData;
	buffer->World = pWorld;
	buffer->View = pView;
	buffer->Projection = pProjection;
	buffer->Scale = pScale;
	buffer->TextureScale = pMaterial.textureScale;
	
	unsigned int jointCount = pSkeleton->getHeader().jointCount;
	for (unsigned int i = 0; i < jointCount; i++)
	{
		AglMatrix am = pSkeleton->getInverseBindMatrix(i) * pSkeleton->getGlobalTransform(i);
		am = AglMatrix::transpose(am);
		buffer->Palette[i] = am;
	}

	mDeviceContext->Unmap(mBuffer, 0);

	unsigned int bufferNumber = 0;
	mDeviceContext->VSSetConstantBuffers(bufferNumber, 1, &mBuffer);

	//Copy the material buffer to the gpu
	mDeviceContext->Map(mMaterialBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);

	MaterialBuffer* matbuffer;
	matbuffer = (MaterialBuffer*)resource.pData;
	matbuffer->AmbientOpacity = AglVector4(pMaterial.ambient, pMaterial.opacity);
	matbuffer->DiffuseReflectivity = AglVector4(pMaterial.diffuse, pMaterial.reflectivity);
	matbuffer->SpecularShininess = AglVector4(pMaterial.specular, pMaterial.shininess);
	matbuffer->EmissiveDiffuseMapped = AglVector4(pMaterial.emissive, float(pMaterial.diffuseTextureNameIndex >= 0));
	matbuffer->EyePositionSpecularMapped = AglVector4(Camera::GetInstance()->Position().x, Camera::GetInstance()->Position().y, Camera::GetInstance()->Position().z, float(pMaterial.specularTextureNameIndex >= 0));
	matbuffer->Flags = AglVector4((float)(pMaterial.glowTextureNameIndex >= 0), (float)(pMaterial.normalTextureNameIndex >= 0), 0, 0);
	mDeviceContext->Unmap(mMaterialBuffer, 0);

	mDeviceContext->PSSetConstantBuffers(bufferNumber, 1, &mMaterialBuffer);

	if (pMaterial.diffuseTextureNameIndex >= 0)
	{
		string s = Scene::GetInstance()->GetName(pMaterial.diffuseTextureNameIndex);
		removePath(s);
		s = Scene::GetInstance()->GetFolder() + s;
		TextureData* data = TextureManager::GetInstance()->GetTexture(s);
		if (data)
			mDeviceContext->PSSetShaderResources(0, 1, &data->SRV);
	}
	if (pMaterial.specularTextureNameIndex >= 0)
	{
		string s = Scene::GetInstance()->GetName(pMaterial.specularTextureNameIndex);
		removePath(s);
		s = Scene::GetInstance()->GetFolder() + s;
		TextureData* data = TextureManager::GetInstance()->GetTexture(s);
		if (data)
			mDeviceContext->PSSetShaderResources(1, 1, &data->SRV);
	}
	if (pMaterial.glowTextureNameIndex >= 0)
	{
		string s = Scene::GetInstance()->GetName(pMaterial.glowTextureNameIndex);
		removePath(s);
		s = Scene::GetInstance()->GetFolder() + s;
		TextureData* data = TextureManager::GetInstance()->GetTexture(s);
		if (data)
			mDeviceContext->PSSetShaderResources(2, 1, &data->SRV);
	}
	if (pMaterial.normalTextureNameIndex >= 0)
	{
		string s = Scene::GetInstance()->GetName(pMaterial.normalTextureNameIndex);
		removePath(s);
		s = Scene::GetInstance()->GetFolder() + s;
		TextureData* data = TextureManager::GetInstance()->GetTexture(s);
		if (data)
			mDeviceContext->PSSetShaderResources(3, 1, &data->SRV);
	}

	mDeviceContext->PSSetSamplers(0, 1, &mSampler);
	mDeviceContext->DSSetSamplers(0, 1, &mSampler);
}