#include "pch.h"
#include "CSceneMgr.h"

#include "CEventMgr.h"
#include "CCollisionMgr.h"

#include "CResMgr.h"
#include "CMesh.h"
#include "CGraphicsShader.h"

#include "CDevice.h"
#include "CPathMgr.h"

#include "CScene.h"
#include "CLayer.h"

#include "CGameObject.h"
#include "CTransform.h"
#include "CMeshRender.h"
#include "CCamera.h"
#include "CCollider2D.h"

#include "CPlayerScript.h"
#include "CCameraMoveScript.h"
#include "CMissileScript.h"

#include "CTexture.h"
#include "CPrefab.h"

#include "CAnimator2D.h"
#include "CAnimation2D.h"


CSceneMgr::CSceneMgr()
	: m_pCurScene(nullptr)
{

}

CSceneMgr::~CSceneMgr()
{
	SAFE_DELETE(m_pCurScene);
}


void CSceneMgr::init()
{
	// ----- Layer 설정 -----  
	m_pCurScene = new CScene;	
	m_pCurScene->SetLayerName(0, L"Default");
	m_pCurScene->SetLayerName(1, L"Player");
	m_pCurScene->SetLayerName(2, L"Monster");
	
	// ----- Texture Load -> CResMgr -----
	LoadTextures();

	// ----- MAKE PREFAB -----
	AddMissilePrefab();

	// ----- MAKE TOOL OBJECT ------
	AddCameraObj();
	
	// ----- MAKE RENDER OBJECT -----
	AddTauromacisObj();
	//AddMainPlayerObj();
	AddMonsterObj();

	



	CCollisionMgr::GetInst()->CollisionCheck(L"Player", L"Monster");

	m_pCurScene->start();
}

void CSceneMgr::progress()
{
	m_pCurScene->update();
	m_pCurScene->lateupdate();
	m_pCurScene->finalupdate();
}

void CSceneMgr::render()
{
	CDevice::GetInst()->ClearTarget();

	//m_pCurScene->render();

	CDevice::GetInst()->Present();
}




void CSceneMgr::SpawnObject(CGameObject* _pSpawnObject, Vec3 _vWorldPos, wstring _strName, UINT _iLayerIdx)
{
	tEventInfo info = {};
	info.eType = EVENT_TYPE::CREATE_OBJ;
	info.lParam = (DWORD_PTR)_pSpawnObject;
	info.wParam = (DWORD_PTR)_iLayerIdx;

	_pSpawnObject->Transform()->SetRelativePos(_vWorldPos);
	_pSpawnObject->SetName(_strName);

	CEventMgr::GetInst()->AddEvent(info);
}

void CSceneMgr::SpawnObject(CGameObject* _pSpawnObject, UINT _iLayerIdx)
{
	tEventInfo info = {};
	info.eType = EVENT_TYPE::CREATE_OBJ;
	info.lParam = (DWORD_PTR)_pSpawnObject;
	info.wParam = (DWORD_PTR)_iLayerIdx;

	CEventMgr::GetInst()->AddEvent(info);
}

void CSceneMgr::AddChild(CGameObject* _pParentObject, CGameObject* _pChildObject)
{
	tEventInfo info = {};
	info.eType = EVENT_TYPE::ADD_CHILD;
	info.lParam = (DWORD_PTR)_pParentObject;
	info.wParam = (DWORD_PTR)_pChildObject;

	CEventMgr::GetInst()->AddEvent(info);
}


void CSceneMgr::LoadTextures()
{
	/*
		필요한 텍스처들을 CResMgr에 로드합니다. 
		->	이후 CResMgr::GetInst()->Findres() 를 통해서
			텍스처 리소스를 받아 사용합니다. 
	*/
	CResMgr::GetInst()->Load<CTexture>(L"PlayerTexture", L"texture\\Player.bmp");
	CResMgr::GetInst()->Load<CTexture>(L"MagicCircle", L"texture\\MagicCircle.png");

	// Monster Texture Load 
	CResMgr::GetInst()->Load<CTexture>(L"Tauromacis", L"texture\\MapleStoryMonster\\Tauromacis.png");


}

void CSceneMgr::AddMainPlayerObj()
{
	Ptr<CTexture> pTex = CResMgr::GetInst()->FindRes<CTexture>(L"MagicCircle");


	// Player Object
	CGameObject* pObject = new CGameObject;
	pObject->SetName(L"Player");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);
	pObject->AddComponent(new CCollider2D);
	pObject->AddComponent(new CAnimator2D);

	pObject->Transform()->SetRelativePos(0.f, 0.f, 500.f);
	pObject->Transform()->SetRelativeScale(Vec3(300.f, 300.f, 1.f));

	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pObject->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"));

	float fLimit = 0.3333f;
	pObject->MeshRender()->GetSharedMaterial()->SetScalarParam(SCALAR_PARAM::FLOAT_0, &fLimit);
	pObject->MeshRender()->GetSharedMaterial()->SetTexParam(TEX_PARAM::TEX_0, pTex.Get());

	pObject->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::CIRCLE);
	pObject->Collider2D()->SetOffsetPos(Vec2(0.f, 0.f));
	pObject->Collider2D()->SetOffsetScale(Vec2(100.f, 100.f));

	Ptr<CTexture> pAnimAtlas = CResMgr::GetInst()->Load<CTexture>(L"PlayerAtlas", L"texture\\link_0.png");
	pObject->Animator2D()->CreateAnim(L"WALK_DOWN", pAnimAtlas, Vec2(200.f, 200.f)
		, Vec2(0.f, 260.f), Vec2(60.f, 65.f), Vec2(60.f, 0.f), 0.2f, 10);




	pObject->Animator2D()->Play(L"WALK_DOWN", true);


	CGameObject* pChildObj = pObject->Clone();
	pChildObj->SetName(L"ChildObject");
	pChildObj->Transform()->SetIgnoreParentScale(true);
	pChildObj->Transform()->SetRelativePos(200.f, 0.f, 0.f);
	pChildObj->Transform()->SetRelativeScale(50.f, 50.f, 50.f);

	pChildObj->Collider2D()->SetOffsetPos(0.f, 0.f);
	pChildObj->Collider2D()->SetOffsetScale(50.f, 50.f);


	pObject->AddChild(pChildObj);
	pObject->AddComponent(new CPlayerScript);

	m_pCurScene->AddObject(pObject, L"Player");


}

void CSceneMgr::AddMonsterObj()
{
	CGameObject* pObject = new CGameObject;

	// Monster Object
	pObject = new CGameObject;
	pObject->SetName(L"Monster");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);
	pObject->AddComponent(new CCollider2D);
	pObject->AddComponent(new CMissileScript);

	pObject->Transform()->SetRelativePos(Vec3(400.f, 0.f, 500.f));
	pObject->Transform()->SetRelativeScale(Vec3(300.f, 300.f, 1.f));

	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pObject->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"));

	pObject->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::CIRCLE);
	pObject->Collider2D()->SetOffsetPos(Vec2(0.f, 0.f));
	pObject->Collider2D()->SetOffsetScale(Vec2(100.f, 100.f));

	m_pCurScene->AddObject(pObject, L"Monster");


}

void CSceneMgr::AddTauromacisObj()
{

	CGameObject* pObj = new CGameObject;
	Ptr<CTexture> pTex = CResMgr::GetInst()->FindRes<CTexture>(L"Tauromacis");


	pObj->SetName(L"Tauromacis");
	pObj->AddComponent(new CTransform);
	pObj->AddComponent(new CMeshRender);
	pObj->AddComponent(new CCollider2D);
	pObj->AddComponent(new CAnimator2D);

	pObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 500.f));
	pObj->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 1.f));


	pObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pObj->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"));

	float fLimit = 0.3333f;
	pObj->MeshRender()->GetSharedMaterial()->SetScalarParam(SCALAR_PARAM::FLOAT_0, &fLimit);
	pObj->MeshRender()->GetSharedMaterial()->SetTexParam(TEX_PARAM::TEX_0, pTex.Get());
	
	pObj->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::CIRCLE);
	pObj->Collider2D()->SetOffsetPos(Vec2(0.f, 0.f));
	pObj->Collider2D()->SetOffsetScale(Vec2(200.f, 200.f));

	pObj->Animator2D()->CreateAnim(L"STOP", pTex, Vec2(200.f, 200.f)
		, Vec2(5.f, 0.f), Vec2(176.f ,160.f), Vec2(186.25f, 0.f), 0.2f, 7);




	pObj->Animator2D()->Play(L"STOP", true);
	m_pCurScene->AddObject(pObj, L"Monster");


	
}

void CSceneMgr::AddCameraObj()
{
	// Camera Object 추가
	CGameObject* pCamObj = new CGameObject;
	pCamObj->AddComponent(new CTransform);
	pCamObj->AddComponent(new CCamera);
	pCamObj->AddComponent(new CCameraMoveScript);

	pCamObj->Camera()->SetProjType(PROJ_TYPE::PERSPECTIVE);
	pCamObj->Camera()->SetCameraAsMain();
	pCamObj->Camera()->CheckLayerMaskAll();


	m_pCurScene->AddObject(pCamObj, L"Default");

}

void CSceneMgr::AddMissilePrefab()
{
	CGameObject* pMissileObj = new CGameObject;
	pMissileObj->AddComponent(new CTransform);
	pMissileObj->AddComponent(new CMeshRender);
	pMissileObj->AddComponent(new CMissileScript);

	pMissileObj->Transform()->SetRelativeScale(Vec3(50.f, 50.f, 1.f));
	pMissileObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CircleMesh"));
	pMissileObj->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TestMtrl"));

	CResMgr::GetInst()->AddRes<CPrefab>(L"MissilePrefab", new CPrefab(pMissileObj));



}




void CSceneMgr::ClearLayer()
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		m_pCurScene->GetLayer(i)->Clear();
	}
}



