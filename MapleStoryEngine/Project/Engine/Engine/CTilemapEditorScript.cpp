#include "pch.h"
#include "CTilemapEditorScript.h"

#include "CKeyMgr.h"
#include "CDevice.h"

#include "CCamera.h"
#include "CLayer.h"
#include "CTileMap.h"
#include "CCamera.h"
#include "CTransform.h"

#include "CSceneMgr.h"

CTilemapEditorScript::CTilemapEditorScript()
	: m_bEditorMode(false)
{
}

CTilemapEditorScript::~CTilemapEditorScript()
{
}


void CTilemapEditorScript::start()
{
}

void CTilemapEditorScript::update()
{
	if (!m_bEditorMode)
		return;

	//if (KEY_PRESSED(KEY::LBTN))
	//{

	//	Vec2 vMousePos = CKeyMgr::GetInst()->GetMousePos(); 
	//	// find Camera Obj !!  
	//	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	//	CLayer* pLayer = pCurScene->GetLayer(L"Default");
	//	CGameObject* pCameraObj = nullptr;

	//	if (nullptr != pLayer)
	//	{
	//		vector<CGameObject*> vObjs = pLayer->GetRootObjects();
	//		for (int i = 0; i < vObjs.size(); ++i)
	//		{
	//			if (vObjs[i]->GetName() == L"MainCamera")
	//			{
	//				pCameraObj = vObjs[i];
	//				break;
	//			}

	//		}
	//	}

	//	if (nullptr != pCameraObj)
	//	{
	//		const Vec3 vCameraPos = pCameraObj->Transform()->GetRelativePos();
	//		float fWidth = pCameraObj->Camera()->GetWidth();
	//		float fHeight = fWidth / pCameraObj->Camera()->GetAspectRatio();

	//		// ���� window ȭ��â���� ���߾��� (0.f ,0.f) �� ���� �������� �� ��ǥ�� 
	//		Vec2 vDx11Pos = Vec2(vMousePos.x - (fWidth / 2.f), (fHeight / 2.f) - vMousePos.y);

	//		// ���� ����󿡼��� ��ǥ�� 
	//		Vec2 vRealPos = vDx11Pos + vCameraPos;

	//		Vec3 TilePos = GetOwner()->Transform()->GetRelativePos();
	//		Vec3 TileScale = GetOwner()->Transform()->GetRelativeScale();

	//		// Ÿ�ϸ� ��ü�� �»�� ��ǥ�� 
	//		Vec3 vTileMapLT = Vec3(TilePos.x - (TileScale.x / 2), TilePos.y + (TileScale.y / 2), TilePos.z);


	//		// Ÿ�ϸ� ���ο� Ŭ���� �־��ٸ� 
	//		if (0 <= vRealPos.x - vTileMapLT.x && vRealPos.x - vTileMapLT.x <= TileScale.x &&
	//			0 <= vTileMapLT.y - vRealPos.y && vTileMapLT.y - vRealPos.y <= TileScale.y)
	//		{
	//			int iCol = (vRealPos.x - vTileMapLT.x) / (GetOwner()->TileMap()->GetTileSize().x);
	//			int iRow = (vTileMapLT.y - vRealPos.y) / (GetOwner()->TileMap()->GetTileSize().y);

	//			int iTileXCount = GetOwner()->TileMap()->GetTileCountX();
	//			UINT iIdx = iTileXCount * iRow + iCol;

	//			// test Ŭ���Ǹ��ϴ� ������ 0���̹��� �ε����� Ÿ�� ���� ** Test =
	//			GetOwner()->TileMap()->SetTileData(iIdx, 0);

	//			
	//		}
	//	}

	//	
	//}

}

void CTilemapEditorScript::lateupdate()
{
}


void CTilemapEditorScript::SaveToFile(FILE* _pFile)
{
}

void CTilemapEditorScript::LoadFromFile(FILE* _pFile)
{
}


