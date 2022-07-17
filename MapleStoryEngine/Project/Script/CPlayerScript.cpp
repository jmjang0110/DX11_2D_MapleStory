#include "pch.h"
#include "CPlayerScript.h"

#include "CMissileScript.h"

#include "CScriptMgr.h"


#include <Engine/CAnimator2D.h>
#include <Engine/CLight2D.h>
#include <Engine/CCollider2D.h>
#include <Engine/CScene.h>
#include <Engine/CLayer.h>
#include <Engine/CSceneMgr.h>
#include <Engine/CPrefab.h>
#include <Engine/CAnimation2D.h>

#include "CRigidBodyScript.h"
#include "CBasicBallScript.h"
#include "CDoubleJumpScript.h"
#include "CSkillScript.h"
CPlayerScript::CPlayerScript()
	: CScript((int)SCRIPT_TYPE::PLAYERSCRIPT)
	//, m_pMissilePrefab(nullptr)
	, m_fSpeed(10.f)
	, m_eDir(PLAYER_DIRECTION::RIGHT)
	, m_eCurState(PLAYER_STATE::IDLE)
	, m_ePrevState(PLAYER_STATE::IDLE)
	, m_bOnGround(false)
	, m_pPrefab(nullptr)
	, m_pDbJumpSubPrefab(nullptr)
	, m_pDbJumpPrefab(nullptr)
{

	SetName(CScriptMgr::GetScriptName(this));


	ClearScriptParam();
	AddScriptParam("PlayerSpeed", SCRIPTPARAM_TYPE::FLOAT, &m_fSpeed);
	AddScriptParam("Direction", SCRIPTPARAM_TYPE::INT, &m_eDir);
	AddScriptParam("PlayerOnGround", SCRIPTPARAM_TYPE::INT, &m_bOnGround);

}

CPlayerScript::CPlayerScript(const CPlayerScript& _origin)
	: CScript((int)SCRIPT_TYPE::PLAYERSCRIPT)
	, m_fSpeed(_origin.m_fSpeed)
	, m_eDir(_origin.m_eDir)
	, m_eCurState(_origin.m_eCurState)
	, m_ePrevState(_origin.m_ePrevState)
	, m_bOnGround(false)
	, m_pPrefab(nullptr)
	, m_pDbJumpSubPrefab(nullptr)
	, m_pDbJumpPrefab(nullptr)

{
	SetName(CScriptMgr::GetScriptName(this));


	ClearScriptParam();
	AddScriptParam("PlayerSpeed", SCRIPTPARAM_TYPE::FLOAT, &m_fSpeed);
	AddScriptParam("Direction", SCRIPTPARAM_TYPE::INT, &m_eDir);
	AddScriptParam("PlayerOnGround", SCRIPTPARAM_TYPE::INT, &m_bOnGround);

}

CPlayerScript::~CPlayerScript()
{
	SAFE_DELETE(m_pPrefab);
	SAFE_DELETE(m_pDbJumpPrefab);
	SAFE_DELETE(m_pDbJumpSubPrefab);

}

void CPlayerScript::start()
{
	//m_pMissilePrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"MissilePrefab");
	m_eDir = PLAYER_DIRECTION::RIGHT;
	m_eCurState = PLAYER_STATE::IDLE;
	m_ePrevState = PLAYER_STATE::IDLE;

	m_bOnGround = false;
	Collider2D()->Reset();

	ClearScriptParam();
	AddScriptParam("PlayerSpeed", SCRIPTPARAM_TYPE::FLOAT, &m_fSpeed);
	AddScriptParam("Direction", SCRIPTPARAM_TYPE::INT, &m_eDir);
	AddScriptParam("PlayerOnGround", SCRIPTPARAM_TYPE::INT, &m_bOnGround);

	m_pPrefab = nullptr;
	m_pDbJumpPrefab = nullptr;
	m_pDbJumpSubPrefab = nullptr;

}

void CPlayerScript::update()
{
	m_vPrevPos = GetOwner()->Transform()->GetRelativePos();

	/*
	* Skill Test
	*/
	if (KEY_TAP(KEY::A))
	{
		CGameObject* ptest = new CGameObject;
		ptest->SetName(L"SkillTest");
		ptest->AddComponent(new CTransform);
		ptest->Transform()->SetRelativePos(GetOwner()->Transform()->GetRelativePos());


		wstring strPrefabKey = L"prefab\\StormPrepare.pref";
		wstring strContent = CPathMgr::GetInst()->GetContentPath();
		wstring FullPath = strContent + strPrefabKey;

		CPrefab* pPrefab1 = new CPrefab;
		pPrefab1->Load(FullPath);
		assert(pPrefab1);

		strPrefabKey = L"prefab\\Storm.pref";
		strContent = CPathMgr::GetInst()->GetContentPath();
		FullPath = strContent + strPrefabKey;

		CPrefab* pPrefab2 = new CPrefab;
		pPrefab2->Load(FullPath);
		assert(pPrefab2);

		strPrefabKey = L"prefab\\StormEnd.pref";
		strContent = CPathMgr::GetInst()->GetContentPath();
		FullPath = strContent + strPrefabKey;

		CPrefab* pPrefab3 = new CPrefab;
		pPrefab3->Load(FullPath);
		assert(pPrefab3);

		CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();

		CSkillScript* pScript = (CSkillScript*)CScriptMgr::GetScript(L"CSkillScript");
		ptest->AddComponent((CComponent*)pScript);

		pScript->SetKey(KEY::A);
		pScript->SetSkillObjByPrefab(pPrefab1, pPrefab2, pPrefab3);
		pScript->SetName(L"stormSkill");

		pCurScene = CSceneMgr::GetInst()->GetCurScene();
		pCurScene->AddObject(ptest, L"Skill");

		SAFE_DELETE(pPrefab1);
		SAFE_DELETE(pPrefab2);
		SAFE_DELETE(pPrefab3);



	}

	Update_State();
	Update_Move();
	Update_Gravity();
	Update_Animation();
	
	m_ePrevState = m_eCurState;
	m_ePrevDir = m_eDir;


	
}


//
//void CPlayerScript::update()
//{
//	/*if (KEY_PRESSED(KEY::Z))
//	{
//		Vec3 vRot = Transform()->GetRelativeRotation();
//		vRot.z += DT * XM_2PI;
//		Transform()->SetRelativeRotation(vRot);
//	}
//
//	if (KEY_TAP(KEY::SPACE))
//	{
//		GetOqwner()->Destroy();
//		GetOwner()->GetChild().at(0)->Destroy();
//
//		GetOwner()->GetChild().at(0)->Destroy();
//		GetOwner()->Destroy();
//
//		GetOwner()->GetChild().at(0)->Destroy();
//		GetOwner()->Destroy();
//
//		if (nullptr != m_pMissilePrefab)
//		{
//			CGameObject* pMissileObject = m_pMissilePrefab->Instantiate();
//
//			Vec3 vMissilePos = Transform()->GetRelativePos();
//			vMissilePos.y += Transform()->GetRelativeScale().y / 2.f;
//
//			CSceneMgr::GetInst()->SpawnObject(pMissileObject, vMissilePos, L"Missile", 0);
//		}		
//	}
//
//	if (KEY_TAP(KEY::B))
//	{
//		m_bBurn = true;
//		Ptr<CMaterial> pMtrl = MeshRender()->GetMaterial();
//		Vec4 vColor(1.f, 0.75f, 0.5f, 0.f);
//		pMtrl->SetScalarParam(SCALAR_PARAM::VEC4_0, &vColor);
//	}
//
//	Burnning();
//	*/
//
//
//	// ===== Todo ========
//	// �̵� ���⿡ ���� CLight2D �� Dir �� �ٲ۴�. 
//	vector<CGameObject*> vecChild = GetOwner()->GetChild();
//	for (int i = 0; i < vecChild.size(); ++i)
//	{
//		if (nullptr != vecChild[i]->GetComponent(COMPONENT_TYPE::LIGHT2D))
//		{
//			Vec3 vDir = GetOwner()->Transform()->GetRelativePos() - PrevPos;
//			if(vDir != Vec3(0.f, 0.f ,0.f))
//				vecChild[i]->Light2D()->SetLightDir(vDir);
//
//		}
//	}
//	PrevPos = GetOwner()->Transform()->GetRelativePos();
//
//}

void CPlayerScript::lateupdate()
{

}

void CPlayerScript::OnCollisionEnter(CGameObject* _OtherObject)
{

}

void CPlayerScript::OnCollision(CGameObject* _OtherObject)
{
}

void CPlayerScript::OnCollisionExit(CGameObject* _OtherObject)
{
}




void CPlayerScript::Update_State()
{

	CRigidBodyScript* pRigid = (CRigidBodyScript*)GetOwner()->GetScriptByName(L"CRigidBodyScript");
	if (nullptr != pRigid)
	{
		float fSpeed = pRigid->GetVelocity().Length();
		if (m_bOnGround && m_ePrevState == PLAYER_STATE::DOUBLE_JUMP)
		{

			Vec3 MaxVelocity = pRigid->GetMaxVelocity();
			MaxVelocity.x = 150.f;
			pRigid->SetMaxVelocity(MaxVelocity);

			m_eCurState = PLAYER_STATE::IDLE;
		}

		if (0.f == fSpeed && m_bOnGround 
			&& m_ePrevState != PLAYER_STATE::ATTACK
			&& m_ePrevState != PLAYER_STATE::ALERT)
		{
			m_eCurState = PLAYER_STATE::IDLE;
			m_eDir = m_ePrevDir;

		}

		if (m_ePrevState == PLAYER_STATE::JUMP)
		{
			if (m_bOnGround == TRUE)
			{
				m_eCurState = PLAYER_STATE::IDLE;
				m_eDir = m_ePrevDir; // TEST 

			}
		}

	}

	if (m_eCurState != PLAYER_STATE::JUMP && m_eCurState != PLAYER_STATE::DOUBLE_JUMP)
	{
		if (KEY_TAP(KEY::LEFT))
		{
			m_eCurState = PLAYER_STATE::WALK;
			m_eDir = PLAYER_DIRECTION::LEFT;
		}
		if (KEY_TAP(KEY::RIGHT))
		{
			m_eCurState = PLAYER_STATE::WALK;
			m_eDir = PLAYER_DIRECTION::RIGHT;
		}

	}
	

	if (KEY_TAP(KEY::SPACE))
	{  
		if (m_eCurState != PLAYER_STATE::JUMP && m_eCurState != PLAYER_STATE::DOUBLE_JUMP)
		{
			m_eCurState = PLAYER_STATE::JUMP;
		}
		else if (m_eCurState == PLAYER_STATE::JUMP)
		{
			m_eCurState = PLAYER_STATE::DOUBLE_JUMP;
			
			// Double Jump - Add Child Obj ( Eff )
			RegisterDoubleJumpEff();

		}

	}


	// KEY_PRESSED 
	if (m_ePrevState == PLAYER_STATE::JUMP || m_ePrevState == PLAYER_STATE::DOUBLE_JUMP)
	{
		if (KEY_PRESSED(KEY::LEFT))
		{
			if(m_bOnGround)
				m_eCurState = PLAYER_STATE::WALK;

 			m_eDir = PLAYER_DIRECTION::LEFT;
		}

		if (KEY_PRESSED(KEY::RIGHT))
		{
			if (m_bOnGround)
				m_eCurState = PLAYER_STATE::WALK;

 			m_eDir = PLAYER_DIRECTION::RIGHT;
		}
	}


	// KEY_AWAY
	if (m_eCurState == PLAYER_STATE::WALK)
	{
		if (KEY_AWAY(KEY::LEFT))
		{
			m_eCurState = PLAYER_STATE::IDLE;
		}
		if (KEY_AWAY(KEY::RIGHT))
		{
			m_eCurState = PLAYER_STATE::IDLE;
		}


	}



	if (m_eCurState == PLAYER_STATE::ATTACK)
	{

		vector<CGameObject*> vecChild = GetOwner()->GetChild();
		for (int i = 0; i < vecChild.size(); ++i)
		{
			if (vecChild[i]->GetName() == L"Body")
			{
				CAnimator2D* pAnimator2D = vecChild[i]->Animator2D();
				CAnimation2D* pAnim = pAnimator2D->GetCurAnim();
				if (pAnim->IsFinish() == true)
				{
					m_eCurState = PLAYER_STATE::ALERT;
					ResetAnim(PLAYER_STATE::ATTACK, GetOwner());

				}
				break;
			}
		}
	}


	if (m_eCurState != PLAYER_STATE::JUMP && m_eCurState != PLAYER_STATE::DOUBLE_JUMP)
	{
		if (KEY_TAP(KEY::LCTRL) || KEY_PRESSED(KEY::LCTRL))
		{
			m_eCurState = PLAYER_STATE::ATTACK;
		}
	}


}

void CPlayerScript::Update_Move()
{
	if (KEY_TAP(KEY::ENTER))
	{
		Vec3 vPos = GetOwner()->Transform()->GetRelativePos();
		GetOwner()->Transform()->SetRelativePos(Vec3(0.f, 0.f, vPos.z));
	}


	CRigidBodyScript* pRigid = (CRigidBodyScript*)GetOwner()->GetScriptByName(L"CRigidBodyScript");
	if (nullptr != pRigid)
	{
		// KEY_TAP
		if (KEY_TAP(KEY::LEFT))
		{
			m_eDir = PLAYER_DIRECTION::LEFT;
			if (m_eCurState != PLAYER_STATE::JUMP && m_eCurState != PLAYER_STATE::DOUBLE_JUMP)
				pRigid->AddVelocity(Vec3(-120.f, 0.f, 0.f));		// �ӵ� ��� ���� 

		}
		if (KEY_TAP(KEY::RIGHT))
		{
			m_eDir = PLAYER_DIRECTION::RIGHT;
			if (m_eCurState != PLAYER_STATE::JUMP && m_eCurState != PLAYER_STATE::DOUBLE_JUMP)
				pRigid->AddVelocity(Vec3(120.f, 0.f, 0.f));

		}


		if (KEY_TAP(KEY::SPACE))
		{			
			if (nullptr != pRigid)
			{
				
				if (m_eCurState == PLAYER_STATE::JUMP)
				{

					pRigid->AddVelocity(Vec3(0.f, 400.f, 0.f));
				}
				else if (m_eCurState == PLAYER_STATE::DOUBLE_JUMP && 
					m_ePrevState != PLAYER_STATE::DOUBLE_JUMP)
				{
					if (m_eDir == PLAYER_DIRECTION::RIGHT)
					{
						pRigid->AddVelocity(Vec3(400.f, 100.f, 0.f)); // ������ Ȯ��
						Vec3 MaxVelocity = pRigid->GetMaxVelocity();
						MaxVelocity.x = 400.f;
						pRigid->SetMaxVelocity(MaxVelocity);
					}
					else if (m_eDir == PLAYER_DIRECTION::LEFT)
					{
						pRigid->AddVelocity(Vec3(-400.f, 100.f, 0.f)); // ������ Ȯ��
						Vec3 MaxVelocity = pRigid->GetMaxVelocity();
						MaxVelocity.x = -400.f;
						pRigid->SetMaxVelocity(MaxVelocity);
					}
		
				}
			}
		}


		// KEY_PRESSED 
		if (m_eCurState != PLAYER_STATE::JUMP && m_eCurState != PLAYER_STATE::DOUBLE_JUMP)
		{
			if (KEY_PRESSED(KEY::LEFT))
			{
				pRigid->AddForce(Vec3(-250.f, 0.f, 0.f));
			}

			if (KEY_PRESSED(KEY::RIGHT))
			{
				 pRigid->AddForce(Vec3(250.f, 0.f, 0.f));
			}
		}
		
		
	}




	// Test  - ATtack 
	if (KEY_TAP(KEY::LCTRL))
	{
		static CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
		static CLayer* pCurLayer = pCurScene->GetLayer(L"Player");



		if (m_pPrefab == nullptr)
		{
			wstring strPrefabKey = L"prefab\\BasicBall.pref";
			wstring strContent = CPathMgr::GetInst()->GetContentPath();
			wstring FullPath = strContent + strPrefabKey;

			m_pPrefab = new CPrefab;
			m_pPrefab->Load(FullPath);
			assert(m_pPrefab);
		}


		// Prefab ���Ͽ� ����� gameObject �� �о �ش� Layer �� �����Ѵ�. 
		CGameObject* NewObj = m_pPrefab->Instantiate();


		int Offset_y = rand() % 100;
		if (Offset_y % 2 == 1)
			Offset_y *= -1;

		Vec3 vPos = GetOwner()->Transform()->GetRelativePos();
		vPos.y += Offset_y;
		NewObj->Transform()->SetRelativePos(vPos);

		CBasicBallScript* pScript = (CBasicBallScript*)CScriptMgr::GetScript(L"CBasicBallScript");

		if (m_eDir == PLAYER_DIRECTION::LEFT)
		{
			pScript->SetDir(BALL_DIRECTION::LEFT);
			NewObj->Animator2D()->Play(L"Move_Left", true);

		}
		else if (m_eDir == PLAYER_DIRECTION::RIGHT)
		{

			pScript->SetDir(BALL_DIRECTION::RIGHT);
			NewObj->Animator2D()->Play(L"Move_Right", true);
		}

		NewObj->AddComponent((CComponent*)pScript);
		pCurScene->AddObject(NewObj, pCurLayer->GetName());

	}

}

void CPlayerScript::Update_Animation()
{
	//if (m_ePrevState == m_eCurState)
	//	return;

	CGameObject* owner = GetOwner();
	vector<CGameObject*> vecChild = GetOwner()->GetChild();
	for (int i = 0; i < vecChild.size(); ++i)
	{
		if (vecChild[i]->Animator2D() == nullptr)
			continue;
		CAnimator2D* pAnimator2D = vecChild[i]->Animator2D();

		Update_Animation(vecChild[i], L"Head");
		Update_Animation(vecChild[i], L"Body");
		Update_Animation(vecChild[i], L"Arm");

	}



	

}

void CPlayerScript::RegisterDoubleJumpEff()
{
	if (m_pDbJumpPrefab == nullptr)
	{
		wstring strPrefabKey = L"prefab\\DoubleJumpEffect.pref";
		wstring strContent = CPathMgr::GetInst()->GetContentPath();
		wstring FullPath = strContent + strPrefabKey;

		m_pDbJumpPrefab = new CPrefab;
		m_pDbJumpPrefab->Load(FullPath);
		assert(m_pDbJumpPrefab);
	}
	if (m_pDbJumpSubPrefab == nullptr)
	{
		wstring strPrefabKey = L"prefab\\DoubleJumpSubEffect.pref";
		wstring strContent = CPathMgr::GetInst()->GetContentPath();
		wstring FullPath = strContent + strPrefabKey;

		m_pDbJumpSubPrefab = new CPrefab;
		m_pDbJumpSubPrefab->Load(FullPath);
		assert(m_pDbJumpSubPrefab);
	}

	// ** m_pDbJumpPrefab ** 
	// Prefab ���Ͽ� ����� gameObject �� �о �ش� Layer �� �����Ѵ�. 
	CGameObject* NewObj = m_pDbJumpPrefab->Instantiate();
	CDoubleJumpScript* pScript = (CDoubleJumpScript*)CScriptMgr::GetScript(L"CDoubleJumpScript");

	if (m_eDir == PLAYER_DIRECTION::LEFT)
	{
		NewObj->Animator2D()->Play(L"Move_Left", false);

	}
	else if (m_eDir == PLAYER_DIRECTION::RIGHT)
	{
		NewObj->Animator2D()->Play(L"Move_Right", false);
	}

	NewObj->AddComponent((CComponent*)pScript);
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	pCurScene->AddObject(NewObj, L"Player");
	CSceneMgr::GetInst()->AddChild(GetOwner(), NewObj);


	// ** m_pDbJumpSubPrefab ** 
	// Prefab ���Ͽ� ����� gameObject �� �о �ش� Layer �� �����Ѵ�. 
	NewObj = m_pDbJumpSubPrefab->Instantiate();
	
	Vec3 vPos = GetOwner()->Transform()->GetRelativePos();

	pScript = (CDoubleJumpScript*)CScriptMgr::GetScript(L"CDoubleJumpScript");

	if (m_eDir == PLAYER_DIRECTION::LEFT)
	{
		vPos.x += 50.f;
		vPos.y += 20.f;
		NewObj->Animator2D()->Play(L"Move_Left", false);

	}
	else if (m_eDir == PLAYER_DIRECTION::RIGHT)
	{
		vPos.y += 20.f;
		vPos.x -= 50.f;
		NewObj->Animator2D()->Play(L"Move_Right", false);
	}
	NewObj->Transform()->SetRelativePos(vPos);
	NewObj->AddComponent((CComponent*)pScript);
	pCurScene = CSceneMgr::GetInst()->GetCurScene();
	pCurScene->AddObject(NewObj, L"Skill");

}

void CPlayerScript::ResetAnim(PLAYER_STATE _eState, CGameObject* _pObj)
{
	if (_eState != PLAYER_STATE::ATTACK)
		return;

	vector<CGameObject*> vecChild = _pObj->GetChild();
	for (int i = 0; i < vecChild.size(); ++i)
	{
		if (vecChild[i]->Animator2D() == nullptr)
			continue;
		CAnimator2D* pAnimator2D = vecChild[i]->Animator2D();
		CAnimation2D* pAnim = pAnimator2D->GetCurAnim();
		if (pAnim->IsFinish() == true)
		{
			pAnim->Reset();

		}
		if (vecChild[i]->GetChild().empty() == false)
		{
			ResetAnim(PLAYER_STATE::ATTACK, vecChild[i]);
		}
	}

}

void CPlayerScript::Update_Animation(CGameObject* _pObj, wstring _name)
{
	if (_pObj->GetName() != _name)
		return;

	m_eMoveDir = m_eDir;
	CAnimator2D* pAnimator2D = _pObj->Animator2D();


	if (_pObj->GetName() == _name)
	{
		switch (m_eCurState)
		{
		case PLAYER_STATE::IDLE:
		{

			if (PLAYER_DIRECTION::LEFT == m_eMoveDir)
			{
				pAnimator2D->Play(L"STAND1_LEFT", true);
			}
			else if (PLAYER_DIRECTION::RIGHT == m_eMoveDir)
			{
				pAnimator2D->Play(L"STAND1_RIGHT", true);

			}

		}
		break;
		case PLAYER_STATE::WALK:
		{
			if (PLAYER_DIRECTION::LEFT == m_eMoveDir)
			{
				pAnimator2D->Play(L"WALK1_LEFT", true);
			}
			else if (PLAYER_DIRECTION::RIGHT == m_eMoveDir)
			{
				pAnimator2D->Play(L"WALK1_RIGHT", true);

			}
		}
		break;
		case PLAYER_STATE::JUMP:
		{
			if (PLAYER_DIRECTION::LEFT == m_eMoveDir)
			{
				
					pAnimator2D->Play(L"JUMP_LEFT", true);
			

			}
			else if (PLAYER_DIRECTION::RIGHT == m_eMoveDir)
			{
				
					pAnimator2D->Play(L"JUMP_RIGHT", true);

			}
		}

		break;

		case PLAYER_STATE::ATTACK:
		{
			if (PLAYER_DIRECTION::LEFT == m_eMoveDir)
			{

				pAnimator2D->Play(L"SWINGO1_LEFT", false);


			}
			else if (PLAYER_DIRECTION::RIGHT == m_eMoveDir)
			{
				// ���� �ȸ��� 
				//pAnimator2D->Play(L"SWINGO1_RIGHT", true);

			}
		}
			break;
		case PLAYER_STATE::ALERT:
		{
			if (PLAYER_DIRECTION::LEFT == m_eMoveDir)
			{

				pAnimator2D->Play(L"ALERT_LEFT", true);


			}
			else if (PLAYER_DIRECTION::RIGHT == m_eMoveDir)
			{
				pAnimator2D->Play(L"ALERT_RIGHT", true);

			}
		}

			break;
		case PLAYER_STATE::DEAD:

			break;
		}

		vector<CGameObject*> ChildChildObj = _pObj->GetChild();
		if (ChildChildObj.empty() == false)
		{
			for (int i = 0; i < ChildChildObj.size(); ++i)
			{
				Update_Animation(ChildChildObj[i], L"Eye");
			}
		}

	}

	
}

void CPlayerScript::Update_Gravity()
{
}





void CPlayerScript::SaveToScene(FILE* _pFile)
{

	CScript::SaveToScene(_pFile);

	fwrite(&m_fSpeed, sizeof(float), 1, _pFile);
	fwrite(&m_bOnGround, sizeof(bool), 1, _pFile);


}

void CPlayerScript::LoadFromScene(FILE* _pFile)
{
	CScript::LoadFromScene(_pFile);


	fread(&m_fSpeed, sizeof(float), 1, _pFile);
	fread(&m_bOnGround, sizeof(bool), 1, _pFile);

}

