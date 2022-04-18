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
	// ----- Layer ���� -----  
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
	AddMainPlayerObj();
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

	_pSpawnObject->Transform()->SetPos(_vWorldPos);
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
		�ʿ��� �ؽ�ó���� CResMgr�� �ε��մϴ�. 
		->	���� CResMgr::GetInst()->Findres() �� ���ؼ�
			�ؽ�ó ���ҽ��� �޾� ����մϴ�. 
	*/
	CResMgr::GetInst()->Load<CTexture>(L"PlayerTexture", L"texture\\Player.bmp");
	CResMgr::GetInst()->Load<CTexture>(L"MagicCircle", L"texture\\MagicCircle.png");


}

void CSceneMgr::AddMainPlayerObj()
{
	Ptr<CTexture> pTex = CResMgr::GetInst()->FindRes<CTexture>(L"MagicCircle");


	//// Player Object
	//CGameObject* pObject = new CGameObject;
	//pObject->SetName(L"Player");
	//pObject->AddComponent(new CTransform);
	//pObject->AddComponent(new CMeshRender);
	//pObject->AddComponent(new CCollider2D);
	//pObject->AddComponent(new CPlayerScript);

	//pObject->Transform()->SetScale(Vec3(300.f, 300.f, 1.f));

	//pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	//pObject->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TestMtrl"));

	////int a = 0;
	////pObject->MeshRender()->GetDynamicMaterial()->SetScalarParam(L"IsColorRed", &a);

	//float fLimit = 0.3333f;
	//pObject->MeshRender()->GetSharedMaterial()->SetScalarParam(SCALAR_PARAM::FLOAT_0, &fLimit);
	//pObject->MeshRender()->GetSharedMaterial()->SetTexParam(TEX_PARAM::TEX_0, pTex.Get());


	//
	////pObject->MeshRender()->GetDynamicMaterial()->SetTexParam(L"OutputTex", pTex);

	//pObject->Collider2D()->SetOffsetPos(Vec2(0.f, 0.f));
	//pObject->Collider2D()->SetOffsetScale(Vec2(100.f, 100.f));

	//m_pCurScene->AddObject(pObject, L"Player");


	// Player Object
	CGameObject* pObject = new CGameObject;
	pObject->SetName(L"Player");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);
	pObject->AddComponent(new CCollider2D);

	pObject->Transform()->SetPos(0.f, 0.f, 500.f);
	pObject->Transform()->SetScale(Vec3(300.f, 300.f, 1.f));

	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pObject->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TestMtrl"));

	float fLimit = 0.3333f;
	pObject->MeshRender()->GetSharedMaterial()->SetScalarParam(SCALAR_PARAM::FLOAT_0, &fLimit);
	pObject->MeshRender()->GetSharedMaterial()->SetTexParam(TEX_PARAM::TEX_0, pTex.Get());

	pObject->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::CIRCLE);
	pObject->Collider2D()->SetOffsetPos(Vec2(0.f, 0.f));
	pObject->Collider2D()->SetOffsetScale(Vec2(100.f, 100.f));

	CGameObject* pChildObj = pObject->Clone();
	pChildObj->SetName(L"ChildObject");
	pChildObj->Transform()->SetIgnoreParentScale(true);
	pChildObj->Transform()->SetPos(200.f, 0.f, 0.f);
	pChildObj->Transform()->SetScale(50.f, 50.f, 50.f);

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

	pObject->Transform()->SetPos(Vec3(400.f, 0.f, 500.f));
	pObject->Transform()->SetScale(Vec3(300.f, 300.f, 1.f));

	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pObject->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TestMtrl"));

	pObject->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::CIRCLE);
	pObject->Collider2D()->SetOffsetPos(Vec2(0.f, 0.f));
	pObject->Collider2D()->SetOffsetScale(Vec2(100.f, 100.f));

	m_pCurScene->AddObject(pObject, L"Monster");


}

void CSceneMgr::AddCameraObj()
{
	// Camera Object �߰�
	CGameObject* pCamObj = new CGameObject;
	pCamObj->AddComponent(new CTransform);
	pCamObj->AddComponent(new CCamera);
	pCamObj->AddComponent(new CCameraMoveScript);

	//pCamObj->Camera()->SetProjType(PROJ_TYPE::PERSPECTIVE);
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

	pMissileObj->Transform()->SetScale(Vec3(50.f, 50.f, 1.f));
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


