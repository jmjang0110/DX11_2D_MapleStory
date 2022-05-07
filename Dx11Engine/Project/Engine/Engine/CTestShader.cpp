#include "pch.h"
#include "CTestShader.h"

CTestShader::CTestShader()
	: CComputeShader(32, 32, 1)
{
}

CTestShader::~CTestShader()
{
}

void CTestShader::UpdateData()
{
	m_pOutputTex->UpdateData_CS(0, false);

	// ȣ�� �׷� ���� ���
	m_iGroupPerThreadCountX;
	m_iGroupPerThreadCountY;

	/*
		!!((UINT)m_pOutputTex->Width() % m_iGroupPerThreadCountX);
		
		->> ���࿡ �������� ������ !! ���ָ� 1 �̹Ƿ� �׷��� �ϳ� �� ���� O [ ex (26!! -> 1 ) ] 
			���࿡ �������� ������ !! ���ָ� 0 �̹Ƿ� ������ �ϳ� �� ���� X [ ex (0 !! -> 0 ) ] 
	*/
	m_iGroupX = (UINT)m_pOutputTex->Width() / m_iGroupPerThreadCountX + !!((UINT)m_pOutputTex->Width() % m_iGroupPerThreadCountX);
	m_iGroupY = (UINT)m_pOutputTex->Height() / m_iGroupPerThreadCountY + !!((UINT)m_pOutputTex->Height() % m_iGroupPerThreadCountY);
	m_iGroupZ = 1;
}
 
void CTestShader::Clear()
{
	CTexture::ClearCS(0);
}
