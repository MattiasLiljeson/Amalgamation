#include "ShaderBase.h"
#include "D3DUtil.h"

ShaderBase::ShaderBase( ShaderVariableContainer p_initData )
{
	m_deviceContext		= p_initData.deviceContext;
	
	m_vertexShader		= p_initData.vertexShader; 
	m_geometryShader	= p_initData.geometryShader;
	m_domainShader		= p_initData.domainShader;
	m_hullShader		= p_initData.hullShader;
	m_pixelShader		= p_initData.pixelShader;

	m_inputLayout		= p_initData.inputLayout;
}

ShaderBase::~ShaderBase()
{
	SAFE_RELEASE(m_inputLayout);

	delete m_vertexShader;
	delete m_geometryShader;
	delete m_domainShader;
	delete m_hullShader;
	delete m_pixelShader;
}

void ShaderBase::applyStages()
{
	if( m_vertexShader ){
		m_deviceContext->VSSetShader( m_vertexShader->data, 0, 0 );
	} else {
		m_deviceContext->VSSetShader( NULL, 0, 0 );
	} 

	if( m_geometryShader ){
		m_deviceContext->GSSetShader( m_geometryShader->data, 0, 0 );
	} else {
		m_deviceContext->GSSetShader( NULL, 0, 0 );
	}

	if( m_domainShader ) {
		m_deviceContext->DSSetShader( m_domainShader->data, 0, 0 );
	} else {
		m_deviceContext->DSSetShader( NULL, 0, 0 );
	}

	if( m_hullShader ) {
		m_deviceContext->HSSetShader( m_hullShader->data, 0, 0 );
	} else {
		m_deviceContext->HSSetShader( NULL, 0, 0 );
	}

	if( m_pixelShader ) {
		m_deviceContext->PSSetShader( m_pixelShader->data, 0, 0 );
	} else {
		m_deviceContext->PSSetShader( NULL, 0, 0 );
	}

	if( m_inputLayout ) {
		m_deviceContext->IASetInputLayout( m_inputLayout );
	}
}
void ShaderBase::unApplyStages()
{
	if( m_vertexShader )
		m_deviceContext->VSSetShader( NULL, 0, 0 );
	if( m_geometryShader )
		m_deviceContext->GSSetShader( NULL, 0, 0 );
	if( m_domainShader )
		m_deviceContext->DSSetShader( NULL, 0, 0 );
	if( m_hullShader )
		m_deviceContext->HSSetShader( NULL, 0, 0 );
	if( m_pixelShader )
		m_deviceContext->PSSetShader( NULL, 0, 0 );

	if( m_inputLayout )
		m_deviceContext->IASetInputLayout( NULL );
}


