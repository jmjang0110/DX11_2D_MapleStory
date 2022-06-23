#include "pch.h"
#include "CCursorScript.h"

#include <Engine/CKeyMgr.h>
#include <Engine/CCamera.h>
#include <Engine/CScene.h>
#include <Engine/CLayer.h>
#include <Engine/CGameObject.h>



CCursorScript::CCursorScript()
	: CScript((int)SCRIPT_TYPE::CURSORSCRIPT)
{
}

CCursorScript::~CCursorScript()
{
}


void CCursorScript::start()
{
}

void CCursorScript::update()
{


	Vec2 vMousePos = CKeyMgr::GetInst()->GetMousePos();

	// find Camera Obj !!  
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	CLayer* pLayer = pCurScene->GetLayer(L"Default");
	CGameObject* pCameraObj = nullptr;

	if (nullptr != pLayer)
	{
		vector<CGameObject*> vObjs = pLayer->GetRootObjects();
		for (int i = 0; i < vObjs.size(); ++i)
		{
			if (vObjs[i]->GetName() == L"MainCamera")
			{
				pCameraObj = vObjs[i];
				break;
			}

		}
	}

	const Vec3 vCameraPos = pCameraObj->Transform()->GetRelativePos();
	float fWidth = pCameraObj->Camera()->GetWidth();
	float fHeight = fWidth / pCameraObj->Camera()->GetAspectRatio();

	// ���� window ȭ��â���� ���߾��� (0.f ,0.f) �� ���� �������� �� ��ǥ�� 
	Vec2 vDx11Pos = Vec2(vMousePos.x - (fWidth / 2.f), (fHeight / 2.f) - vMousePos.y);

	// ���� ����󿡼��� ��ǥ�� 
	Vec2 vRealPos = vDx11Pos + vCameraPos;

	GetOwner()->Transform()->SetRelativePos(Vec3(vRealPos.x, vRealPos.y, 0.f));



}

void CCursorScript::lateupdate()
{
}

void CCursorScript::OnCollisionEnter(CGameObject* _OtherObject)
{
}
