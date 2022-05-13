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
	m_ParticleBuffer->UpdateData_CS(0, false); // register u0 ��  bind �ϰڴ�
	m_ParticleDataBuffer->UpdateData_CS(1, false); // register u1 ��  bind �ϰڴ�

	// �׷�� Threads ���� : 32��  
	// 32 ���� �� ��� �׷��� ������
	// ��׵��� �� Ŀ�� �� �� ���� ��ŭ�� thread �� �ٳ� ?
	m_iGroupX = m_ParticleBuffer->GetElementCount() / m_iGroupPerThreadCountX + !!(m_ParticleBuffer->GetElementCount() % m_iGroupPerThreadCountX);
	m_iGroupY = 1;
	m_iGroupZ = 1;

	// Max Thread Count
	m_Param.iArr[0] = m_iGroupX * m_iGroupPerThreadCountX;
	m_Param.iArr[1] = m_ParticleBuffer->GetElementCount();

}

void CParticleUpdateShader::Clear()
{
	m_ParticleBuffer->Clear();
}