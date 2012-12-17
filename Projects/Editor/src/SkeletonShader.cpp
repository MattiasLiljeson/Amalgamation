#include "SkeletonShader.h"
#include "Scene.h"

SkeletonShader::SkeletonShader(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, VertexShader* pVS, PixelShader* pPS)
{
	mDevice			= pDevice;
	mDeviceContext	= pDeviceContext;

	mVS = pVS;
	mPS = pPS;

	D3D11_INPUT_ELEMENT_DESC desc[] =
	{
		{"INDEX", 0, DXGI_FORMAT_R32_UINT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	mDevice->CreateInputLayout(desc, 1, mVS->GetCompiledData(), mVS->GetCompiledSize(), &mIL);

	D3D11_BUFFER_DESC BufferDesc;
	BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	BufferDesc.ByteWidth = sizeof(SkeletonShaderBuffer);
	BufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	BufferDesc.MiscFlags = 0;
	BufferDesc.StructureByteStride = 0;
	pDevice->CreateBuffer(&BufferDesc, NULL, &mBuffer);
}
SkeletonShader::~SkeletonShader()
{
	mIL->Release();
	mBuffer->Release();

}
ID3D11VertexShader* SkeletonShader::GetVertexShader()
{
	return mVS->GetShader();
}
ID3D11PixelShader*	SkeletonShader::GetPixelShader()
{
	return mPS->GetShader();
}
ID3D11InputLayout* SkeletonShader::GetInputLayout()
{
	return mIL;
}
void SkeletonShader::SetBuffer(AglMatrix pWorld, AglMatrix pView, AglMatrix pProjection, float pScale, SkeletonMesh* pSkeletonMesh)
{
	pWorld = AglMatrix::transpose(pWorld);
	pView = AglMatrix::transpose(pView);
	pProjection = AglMatrix::transpose(pProjection);

	//Copy the matrix palette buffer to the gpu
	D3D11_MAPPED_SUBRESOURCE resource;
	mDeviceContext->Map(mBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);

	SkeletonShaderBuffer* buffer;
	buffer = (SkeletonShaderBuffer*)resource.pData;
	buffer->World = pWorld;
	buffer->View = pView;
	buffer->Projection = pProjection;
	buffer->Scale = pScale;
	
	AglSkeleton* s = pSkeletonMesh->GetSkeleton();
	unsigned int jointCount = s->getHeader().jointCount;

	AglMatrix inv = Scene::m_avoidJump.inverse();
	for (unsigned int i = 0; i < jointCount; i++)
	{
		AglMatrix am = s->getGlobalTransform(i);
		//am *= inv;
		am = AglMatrix::transpose(am);
		buffer->Palette[i] = am;
	}

	mDeviceContext->Unmap(mBuffer, 0);

	unsigned int bufferNumber = 0;
	mDeviceContext->VSSetConstantBuffers(bufferNumber, 1, &mBuffer);
}