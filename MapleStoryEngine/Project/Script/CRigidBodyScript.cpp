#include "pch.h"
#include "CRigidBodyScript.h"

#include <Engine/CTimeMgr.h>

#include <Engine/CTransform.h>


CRigidBodyScript::CRigidBodyScript()
	: CScript((int)SCRIPT_TYPE::RIGIDBODYSCRIPT)
	, m_fMass(1.f)
	, m_fFricCoeff(100.f)
	, m_fMaxSpeed(200.f)

{
	SetName(CScriptMgr::GetScriptName(this));

}

CRigidBodyScript::~CRigidBodyScript()
{

}

void CRigidBodyScript::start()
{

}

void CRigidBodyScript::update()
{

}

void CRigidBodyScript::lateupdate()
{
	if (0.f != m_vForce.Length())
	{
		// Velocity 
		m_vAccel = m_vForce / m_fMass;
		m_vVelocity += m_vAccel * DT;					// ���ӵ��� ���� �ӵ� ��ȭ 
	}

	if (!m_vVelocity.IsZero())
	{
		Vec3 vFricDir = -m_vVelocity;
		vFricDir.Normalize();
		Vec3 vFriction = vFricDir * m_fFricCoeff * DT;  // ������ ���

		if (m_vVelocity.Length() < vFriction.Length())
		{
			m_vVelocity = Vec3(0.f, 0.f, 0.f);			// �����¿� ���� ���� 
		}
		else
		{
			m_vVelocity += vFriction;					// ������ ����  
		}
	}

	if (m_fMaxSpeed < m_vVelocity.Length())
	{
		m_vVelocity.Normalize();
		m_vVelocity *= m_fMaxSpeed;						// �ִ� �ӷ� ���� 
	}

	// Move 
	float fSpeed = m_vVelocity.Length();				// �̵� �ӷ� 
	if (0.f != fSpeed)
	{
		Vec3 vDir = m_vVelocity;
		vDir.Normalize();								// �̵� ���� 

		Vec3 vPos = Transform()->GetRelativePos();
		vPos += m_vVelocity * DT;						// vPos += vDir * fSpeed * DT;
		Transform()->SetRelativePos(vPos);
	}


	m_vForce = Vec3(0.f, 0.f, 0.f);						// �� �ʱ�ȭ 

}

void CRigidBodyScript::OnCollisionEnter(CGameObject* _OtherObject)
{
}

void CRigidBodyScript::SaveToScene(FILE* _pFile)
{
}

void CRigidBodyScript::LoadFromScene(FILE* _pFile)
{
}

