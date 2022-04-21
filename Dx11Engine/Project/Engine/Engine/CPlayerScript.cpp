#include "pch.h"
#include "CPlayerScript.h"

#include "CMissileScript.h"




CPlayerScript::CPlayerScript()
	: m_pMissilePrefab(nullptr)
	, m_fSpeed(0.5f)
{
	m_JumpTool.start();

}

CPlayerScript::~CPlayerScript()
{

}

void CPlayerScript::start()
{
	m_pMissilePrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"MissilePrefab");
}

void CPlayerScript::update()
{
	
	Vec3 vPos = Transform()->GetRelativePos();

	if (KEY_PRESSED(KEY::LEFT))
		vPos.x -= DT * 100.f;

	if (KEY_PRESSED(KEY::RIGHT))
		vPos.x += DT * 100.f;

	if (KEY_TAP(KEY::SPACE) && !m_JumpTool._IsJumping)
	{
		m_JumpTool._IsJumping = true;
		m_JumpTool._TransformPos = vPos;
		m_JumpTool._StartPosY = vPos.y;
	}
	if (m_JumpTool._IsJumping)
	{
		Jump();
	}


	if(m_JumpTool._IsJumping == false)
		Transform()->SetRelativePos(vPos);

	if (KEY_PRESSED(KEY::Z))
	{
		Vec3 vRot = Transform()->GetRelativeRotation();
		vRot.z += DT * XM_2PI;
		Transform()->SetRelativeRotation(vRot);
	}
	/*
	if (KEY_TAP(KEY::SPACE))
	{
		if (nullptr != m_pMissilePrefab)
		{
			CGameObject* pMissileObject = m_pMissilePrefab->Instantiate();

			Vec3 vMissilePos = Transform()->GetPos();
			vMissilePos.y += Transform()->GetScale().y / 2.f;

			CSceneMgr::GetInst()->SpawnObject(pMissileObject, vMissilePos, L"Missile", 0);
		}		
	}*/
}

void CPlayerScript::lateupdate()
{
	
}

void CPlayerScript::OnCollisionEnter(CGameObject* _OtherObject)
{
	if (_OtherObject->GetScript() == nullptr)
		return;

	if (_OtherObject->GetName() == L"Missile")
	{
		int a = 0;
	}
	
	// C++ RTTI �� �̿��� Ÿ�� ����
	if (typeid(CMissileScript).hash_code() == typeid(*_OtherObject->GetScript()).hash_code())
	{
		int a = 0;
	}

	CMissileScript* pMissileScript = dynamic_cast<CMissileScript*>(_OtherObject->GetScript());
	if (pMissileScript)
	{
		int a = 0;
	}
}



void CPlayerScript::Jump()
{
	// y = a * x + b  ���� ( a  : �߷� ���ӵ� , b : �ʱ� ���� �ӵ� )
	// �����Ͽ� y = ( -a / 2 ) * x *x  + ( b * x ) ������ ��´�. ( x : �����ð�, y : ������Ʈ�� ���� )
	// ��ȭ�� ���� hegith �� ���� ���� _StartPosY �� ���Ѵ�. 

	float Height = (m_JumpTool._JumpTime * m_JumpTool._JumpTime * (-m_JumpTool._Gravity / 2) 
		+ (m_JumpTool._JumpTime * m_JumpTool._JumpPower));
	
	//Height *= 100.f;

	Vec3 vPos = Transform()->GetRelativePos();
	vPos = Vec3(vPos.x, vPos.y + Height, vPos.z);

	m_JumpTool._JumpTime += DT ;


	if (Height < 0.0f)
	{
		m_JumpTool._IsJumping = false;
		m_JumpTool._JumpTime = 0.f;
	}

	Transform()->SetRelativePos(vPos);

}




