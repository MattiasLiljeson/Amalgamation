#include "SkeletonMeshShader.h"
#include "AGLLoader.h"
#include "Camera.h"
#include "TextureManager.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "Scene.h"
#include "Globals.h"

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


	//NEW! Sample matrices from texture
	createSkeletonTexture();

	D3D11_SAMPLER_DESC samplerDescBones;
	samplerDescBones.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	samplerDescBones.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDescBones.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDescBones.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDescBones.MipLODBias = 0.0f;
	samplerDescBones.MaxAnisotropy = 1;
	samplerDescBones.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDescBones.BorderColor[0] = 0;
	samplerDescBones.BorderColor[1] = 0;
	samplerDescBones.BorderColor[2] = 0;
	samplerDescBones.BorderColor[3] = 0;
	samplerDescBones.MinLOD = 0;
	samplerDescBones.MaxLOD = D3D11_FLOAT32_MAX;
	pDevice->CreateSamplerState(&samplerDescBones, &mBoneTextureSampler);
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
	matbuffer->EmissiveDiffuseMapped = AglVector4(pMaterial.emissive, float(pMaterial.diffuseTextureNameIndex >= 0 && DIFFUSEON));
	matbuffer->EyePositionSpecularMapped = AglVector4(Camera::GetInstance()->Position().x, Camera::GetInstance()->Position().y, Camera::GetInstance()->Position().z, float(pMaterial.specularTextureNameIndex >= 0 && SPECULARON));
	matbuffer->Flags = AglVector4((float)(pMaterial.glowTextureNameIndex >= 0 && GLOWON), (float)(pMaterial.normalTextureNameIndex >= 0  && NORMALON), 0, 0);
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
			mDeviceContext->PSSetShaderResources(3, 1, &data->SRV);
	}

	mDeviceContext->PSSetSamplers(0, 1, &mSampler);
	mDeviceContext->DSSetSamplers(0, 1, &mSampler);


	//Bone texture data
	mDeviceContext->VSSetSamplers(0, 1, &mBoneTextureSampler);

	D3D11_MAPPED_SUBRESOURCE mappedTex;
	mDeviceContext->Map(mBoneTexture, D3D11CalcSubresource(0, 0, 1), D3D11_MAP_WRITE_DISCARD, 0, &mappedTex);

	float* values = (float*)mappedTex.pData;

	jointCount = pSkeleton->getHeader().jointCount;
	for (unsigned int i = 0; i < jointCount; i++)
	{
		AglMatrix am = pSkeleton->getInverseBindMatrix(i) * pSkeleton->getGlobalTransform(i);
		//am = AglMatrix::transpose(am);

		int pos = i*16;

		//Row1
		values[pos] = am[0];
		values[pos+1] = am[1];
		values[pos+2] = am[2];
		values[pos+3] = am[3];
		//Row2
		values[pos+4] = am[4];
		values[pos+5] = am[5];
		values[pos+6] = am[6]; 
		values[pos+7] = am[7]; 
		//Row3
		values[pos+8] = am[8];
		values[pos+9] = am[9];
		values[pos+10] =am[10];
		values[pos+11] =am[11];
		//Row4
		values[pos+12] = am[12];
		values[pos+13] = am[13];
		values[pos+14] = am[14];
		values[pos+15] = am[15];
	}

	mDeviceContext->Unmap(mBoneTexture, D3D11CalcSubresource(0, 0, 1));
	mDeviceContext->VSSetShaderResources(0, 1, &mBoneSRV);
}

void SkeletonMeshShader::createSkeletonTexture()
{
	D3D11_TEXTURE1D_DESC desc;
	desc.MiscFlags = 0;
	desc.Width = 16384; // 2^14
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT; //One row in the matrix
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	HRESULT res = mDevice->CreateTexture1D(&desc, NULL, &mBoneTexture );

	D3D11_SHADER_RESOURCE_VIEW_DESC srDesc;
	srDesc.Format = desc.Format;
	srDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE1D;
	srDesc.Texture2D.MostDetailedMip = 0;
	srDesc.Texture2D.MipLevels = 1;
	mDevice->CreateShaderResourceView(mBoneTexture, &srDesc, &mBoneSRV );



	D3D11_MAPPED_SUBRESOURCE mappedTex;
	res = mDeviceContext->Map(mBoneTexture, D3D11CalcSubresource(0, 0, 1), D3D11_MAP_WRITE_DISCARD, 0, &mappedTex);

	float* values = (float*)mappedTex.pData;
	for( UINT col = 0; col < desc.Width; )
	{
		//Row1
		values[col++] = 1;
		values[col++] = 0;
		values[col++] = 0; 
		values[col++] = 0; 
		//Row2
		values[col++] = 0;
		values[col++] = 1;
		values[col++] = 0; 
		values[col++] = 0; 
		//Row3
		values[col++] = 0;
		values[col++] = 0;
		values[col++] = 1; 
		values[col++] = 0; 
		//Row4
		values[col++] = 0;
		values[col++] = 0;
		values[col++] = 0; 
		values[col++] = 1; 
	}

	mDeviceContext->Unmap(mBoneTexture, D3D11CalcSubresource(0, 0, 1));


}