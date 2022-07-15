#include "pch.h"
#include "CRigidBodyScript.h"
#include "CPlayerScript.h" 
#include "CGravityScript.h"

#include <Engine/CTimeMgr.h>
#include <Engine/CTransform.h>


CRigidBodyScript::CRigidBodyScript()
	: CScript((int)SCRIPT_TYPE::RIGIDBODYSCRIPT)
	, m_vForce(Vec3(0.f, 0.f, 0.f))
	, m_vAccel(Vec3(0.f, 0.f, 0.f))
	, m_vAccelA(Vec3(0.f, 0.f, 0.f))
	, m_vVelocity(Vec3(0.f, 0.f, 0.f))
	, m_vMaxVelocity(Vec3(150.f, 400.f, 0.f))
	, m_fMass(1.f)
	, m_fFricCoeff(200.f)

{
	SetName(CScriptMgr::GetScriptName(this));

}

CRigidBodyScript::CRigidBodyScript(const CRigidBodyScript& _origin)
	: CScript((int)SCRIPT_TYPE::RIGIDBODYSCRIPT)
	, m_vForce(Vec3(0.f, 0.f, 0.f))
	, m_vAccel(Vec3(0.f, 0.f, 0.f))
	, m_vAccelA(Vec3(0.f, 0.f, 0.f))
	, m_vVelocity(Vec3(0.f, 0.f, 0.f))
	, m_vMaxVelocity(Vec3(150.f, 400.f, 0.f))
	, m_fMass(1.f)
	, m_fFricCoeff(200.f)
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


	if (0.f != m_vForce.Length())
	{
		// Velocity 
		m_vAccel = m_vForce / m_fMass;
	}

	m_vAccel += m_vAccelA;								// �߰� ���ӵ� ex) �߷� 
	m_vVelocity += m_vAccel * DT;						// ���ӵ��� ���� �ӵ� ��ȭ 

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

  
	if (abs(m_vMaxVelocity.x) < abs(m_vVelocity.x))
	{
		m_vVelocity.x = (m_vVelocity.x / abs(m_vVelocity.x)) * abs(m_vMaxVelocity.x);	// �ִ� �ӵ�  ���� (x��)
	}
	if (abs(m_vMaxVelocity.y) < abs(m_vVelocity.y))
	{
		m_vVelocity.y = (m_vVelocity.y / abs(m_vVelocity.y)) * abs(m_vMaxVelocity.y);	// �ִ� �ӵ�  ���� (y��)
	}
	if (abs(m_vMaxVelocity.z) < abs(m_vVelocity.z))
	{
		m_vVelocity.z = (m_vVelocity.z / abs(m_vVelocity.z)) * abs(m_vMaxVelocity.z);	// �ִ� �ӵ�  ���� (z��)
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
	m_vAccel = Vec3(0.f, 0.f, 0.f);						// ���ӵ� �ʱ�ȭ 
	m_vAccelA = Vec3(0.f, 0.f, 0.f);					// �߰� ���ӵ� �ʱ�ȭ
}

void CRigidBodyScript::lateupdate()
{
	

}

void CRigidBodyScript::OnCollisionEnter(CGameObject* _OtherObject)
{
}

void CRigidBodyScript::OnCollision(CGameObject* _OtherObject)
{
}

void CRigidBodyScript::OnCollisionExit(CGameObject* _OtherObject)
{
}

void CRigidBodyScript::SaveToScene(FILE* _pFile)
{
}

void CRigidBodyScript::LoadFromScene(FILE* _pFile)
{
}

