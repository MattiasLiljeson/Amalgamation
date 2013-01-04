#include "GradientShader.h"
#include "Camera.h"
#include "TextureManager.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "Scene.h"

GradientShader::GradientShader(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, VertexShader* pVS, PixelShader* pPS)
{
	mDevice			= pDevice;
	mDeviceContext	= pDeviceContext;

	mVS = pVS;
	mPS = pPS;

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
	BufferDesc.ByteWidth = sizeof(GradientShaderBuffer);
	BufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	BufferDesc.MiscFlags = 0;
	BufferDesc.StructureByteStride = 0;
	pDevice->CreateBuffer(&BufferDesc, NULL, &mBuffer);

	BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	BufferDesc.ByteWidth = sizeof(GradientBuffer);
	BufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	BufferDesc.MiscFlags = 0;
	BufferDesc.StructureByteStride = 0;
	pDevice->CreateBuffer(&BufferDesc, NULL, &mGradientBuffer);
}
GradientShader::~GradientShader()
{
	mIL->Release();
	mBuffer->Release();
	mGradientBuffer->Release();

}
ID3D11VertexShader* GradientShader::GetVertexShader()
{
	return mVS->GetShader();
}
ID3D11PixelShader*	GradientShader::GetPixelShader()
{
	return mPS->GetShader();
}
ID3D11InputLayout* GradientShader::GetInputLayout()
{
	return mIL;
}
void GradientShader::SetBuffer(AglMatrix pWorld, AglMatrix pView, AglMatrix pProjection, float pScale, AglGradient* pGradient)
{
	pWorld = AglMatrix::transpose(pWorld);
	pView = AglMatrix::transpose(pView);
	pProjection = AglMatrix::transpose(pProjection);

	D3D11_MAPPED_SUBRESOURCE resource;
	mDeviceContext->Map(mBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);

	GradientShaderBuffer* buffer;
	buffer = (GradientShaderBuffer*)resource.pData;
	buffer->World = pWorld;
	buffer->View = pView;
	buffer->Projection = pProjection;
	buffer->Scale = pScale;

	mDeviceContext->Unmap(mBuffer, 0);

	unsigned int bufferNumber = 0;
	mDeviceContext->VSSetConstantBuffers(bufferNumber, 1, &mBuffer);

	//Copy the material buffer to the gpu
	mDeviceContext->Map(mGradientBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);

	GradientBuffer* matbuffer;
	matbuffer = (GradientBuffer*)resource.pData;

	vector<AglGradientMaterial*> layers = pGradient->getLayers();
	for (unsigned int i = 0; i < layers.size(); i++)
	{
		matbuffer->AmbientOpacity[i] = AglVector4(layers[i]->ambient, 1);
		matbuffer->DiffuseReflectivity[i] = AglVector4(layers[i]->diffuse, 0);
	}
	matbuffer->Flags = AglVector4(pGradient->getNormalTextureNameIndex() >= 0, (float)layers.size(), 0, 0);

	mDeviceContext->Unmap(mGradientBuffer, 0);

	mDeviceContext->PSSetConstantBuffers(bufferNumber, 1, &mGradientBuffer);

	if (pGradient->getGradientTextureNameIndex() >= 0)
	{
		string s = Scene::GetInstance()->GetName(pGradient->getGradientTextureNameIndex());
		TextureData* td = TextureManager::GetInstance()->GetTexture(s);
		if (td)
			mDeviceContext->PSSetShaderResources(0, 1, &td->SRV);
	}
	else
	{
	}
	if (pGradient->getNormalTextureNameIndex() >= 0)
	{
		string s = Scene::GetInstance()->GetName(pGradient->getNormalTextureNameIndex());
		TextureData* td = TextureManager::GetInstance()->GetTexture(s);
		if (td)
			mDeviceContext->PSSetShaderResources(1, 1, &td->SRV);
	}
}