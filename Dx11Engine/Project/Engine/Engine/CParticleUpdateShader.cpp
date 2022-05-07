#include "pch.h"
#include "CParticleUpdateShader.h"


CParticleUpdateShader::CParticleUpdateShader()
	: CComputeShader(32, 1, 1) // X �� : 32��  32 ���� ¥�� �׷�
	, m_ParticleBuffer(nullptr)
{
}

CParticleUpdateShader::~CParticleUpdateShader()
{
}

void CParticleUpdateShader::UpdateData()
{
	m_ParticleBuffer->UpdateData_CS(0, false); // register u ��  bind �ϰڴ�

	// 32 ���� �� ��� �׷��� ������
	// ��׵��� �� Ŀ�� �� �� ���� ��ŭ�� thread �� �ٳ� ?
	m_iGroupX = m_ParticleBuffer->GetElementCount() / m_iGroupPerThreadCountX + !!(m_ParticleBuffer->GetElementCount() % m_iGroupPerThreadCountX);
	m_iGroupY = 1;
	m_iGroupZ = 1;
}

void CParticleUpdateShader::Clear()
{
	m_ParticleBuffer->Clear();
}