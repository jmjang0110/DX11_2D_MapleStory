#include "pch.h"
#include "TileMapUI.h"

#include "TreeUI.h"

#include <Engine/CScene.h>
#include <Engine/CLayer.h>


#include <Engine/CTileMap.h>
#include <Engine/CTexture.h>
#include <Engine/CTransform.h>
#include <Engine/CCamera.h>


#include <Engine/CSceneMgr.h>
#include <Engine/CPathMgr.h>
#include <Engine/CKeyMgr.h>
#include <Engine/CResMgr.h>


/*
	
	* ������ ���丮 Ÿ���� ����Ŷ� �� ĭ�� 90x60 ���� ������̴�. 
	
	* Ʈ�� �� ����ؼ� Ÿ���� ���� �� �ֵ��� �����
	
	* tile info �� �������� �����ؼ� Ÿ���� ������ �� Ÿ���� �̹����� ����
	  ������ ��� ��
	 
	 * �̹����� ���� ���ؼ� �̸� Ÿ�� ����� ������ ���� �� 
	

*/

TileMapUI::TileMapUI()
	: ComponentUI("TileMapUI", COMPONENT_TYPE::TILEMAP)
	, m_TreeUI(nullptr)
	, m_iMapCountX(0)
	, m_iMapCountY(0)
	, m_pTargetObject(nullptr)
	, m_bEditMode(false)

{
	SetSize(Vec2(0.f, 300.f));


	m_TreeUI = new TreeUI(true);
	m_TreeUI->SetTitle("Tile");
	m_TreeUI->UseFrame(false);
	m_TreeUI->ShowDummyRoot(false);

	m_TreeUI->UseDragDropSelf(false);
	m_TreeUI->UseDragDropOuter(false);

	//AddChild(m_TreeUI);


	m_TreeUI->SetClickedDelegate(this, (CLICKED)&TileMapUI::TileClicked);
	




	Reset();

}

TileMapUI::~TileMapUI()
{

}

void TileMapUI::update()
{
	ComponentUI::update();




}

void TileMapUI::render_update()
{
	ComponentUI::render_update();

	if (KEY_TAP(KEY::X))
		int i = 0;

	CGameObject* pObject = GetTargetObject();

	if (nullptr == m_pTargetObject)
	{
		m_pTargetObject = pObject;

		m_iMapCountX = pObject->TileMap()->GetTileCountX();
		m_iMapCountY = pObject->TileMap()->GetTileCountY();

	}


	// TileCount
	ImGui::PushItemWidth(100);
	ImGui::Text("Tile Count");
	ImGui::SameLine(100);
	ImGui::InputInt("##TileCountX", &m_iMapCountX);
	ImGui::SameLine();
	ImGui::InputInt("##TileCountY", &m_iMapCountY);


	if (ImGui::Button("Setting"))
	{
		int i = 0;

		m_pTargetObject->TileMap()->SetTileMapCount(m_iMapCountX, m_iMapCountY);
		// Ÿ�� ���� / ������ ��ŭ Ÿ�ϸ� ũ�⸦ �÷����Ѵ�. 
		Vec2 SlicePixel = m_pTargetObject->TileMap()->GetTileSize();
		m_pTargetObject->Transform()->SetRelativeScale(Vec3(m_iMapCountX * SlicePixel.x, m_iMapCountY * SlicePixel.y, 1.f));


	}
	
	ImGui::PopItemWidth();


	// Slice pixel Size 
	ImGui::PushItemWidth(200);
	Vec2 SlicePixel = m_pTargetObject->TileMap()->GetTileSize();
	float pxSlice[2] = { SlicePixel.x , SlicePixel.y };

	ImGui::Text("Tile px Size");
	ImGui::SameLine(100);
	ImGui::InputFloat2("##TilepxSize", &pxSlice[0]);
	m_pTargetObject->TileMap()->SetTileSize(Vec2(pxSlice[0], pxSlice[1]));
	ImGui::PopItemWidth();


	// Tilemap list

	
	m_TreeUI->render();
	

	// EditorMode Button 
	ImGui::Checkbox("EditorMode", &m_bEditMode);
	ImGui::Checkbox("Show Grid", &m_bShowGrid);
	ImGui::Checkbox("Auto Set Next Tile", &m_bAutoSetNextTile);


	if (m_bShowGrid == true)
		GetTargetObject()->TileMap()->SetShowGrid(1);
	else
		GetTargetObject()->TileMap()->SetShowGrid(0);

	if(m_bEditMode)
		EditorUpdate();


	//ImGui::BeginChild("Selected Tile", ImVec2(200.f, 200.f), true, ImGuiWindowFlags_HorizontalScrollbar);
	ImDrawList* draw_list = ImGui::GetWindowDrawList();


	if (nullptr != m_pSelected_Tile)
	{
		Ptr<CTexture> SelectedTile_Atlas = m_pSelected_Tile->_parent->_parent->pAtlasTex;
		ImTextureID _texid = SelectedTile_Atlas->GetSRV().Get();

		int MaxRow = SelectedTile_Atlas->Height() / 30.f;  // ��
		int MaxCol = SelectedTile_Atlas->Width() /  30.f ; // �� 

		ImVec2 size(30.f * m_pSelected_Tile->iCol, 30.f * m_pSelected_Tile->iRow);
		ImGui::InvisibleButton("##empty", size);
		const ImVec2 p0 = ImGui::GetItemRectMin();
		const ImVec2 p1 = ImGui::GetItemRectMax();

		ImGui::PushClipRect(p0, p1, true);
		draw_list->AddRect(p0, p1, IM_COL32(120, 120, 120, 255)); // RGBA

		int imgeIDx = m_pSelected_Tile->vTilesInfo[0].iImgIdx; // ó������ ���� a
		Vec2 vLT = Vec2((imgeIDx % MaxCol) * 30.f, (imgeIDx / MaxCol) * 30.f);
		Vec2 vSlice = Vec2(size.x, size.y);

		ImVec2 uv0 = ImVec2((vLT.x) / SelectedTile_Atlas->Width(), (vLT.y) / SelectedTile_Atlas->Height());
		ImVec2 uv1 = ImVec2((vLT.x + vSlice.x) / SelectedTile_Atlas->Width()
			, (vLT.y + vSlice.y) / SelectedTile_Atlas->Height());

		draw_list->AddImage(_texid, p0, p1, uv0, uv1);


		ImGui::PopClipRect();
	}
	//ImGui::EndChild();


}

void TileMapUI::EditorUpdate()
{

	if (KEY_PRESSED(KEY::LBTN))
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

		if (nullptr != pCameraObj)
		{
			const Vec3 vCameraPos = pCameraObj->Transform()->GetRelativePos();
			float fWidth = pCameraObj->Camera()->GetWidth();
			float fHeight = fWidth / pCameraObj->Camera()->GetAspectRatio();

			// ���� window ȭ��â���� ���߾��� (0.f ,0.f) �� ���� �������� �� ��ǥ�� 
			Vec2 vDx11Pos = Vec2(vMousePos.x - (fWidth / 2.f), (fHeight / 2.f) - vMousePos.y);

			// ���� ����󿡼��� ��ǥ�� 
			Vec2 vRealPos = vDx11Pos + vCameraPos;

			Vec3 TilePos = GetTargetObject()->Transform()->GetRelativePos();
			Vec3 TileScale = GetTargetObject()->Transform()->GetRelativeScale();

			// Ÿ�ϸ� ��ü�� �»�� ��ǥ�� 
			Vec3 vTileMapLT = Vec3(TilePos.x - (TileScale.x / 2), TilePos.y + (TileScale.y / 2), TilePos.z);


			// Ÿ�ϸ� ���ο� Ŭ���� �־��ٸ� 
			if (0 <= vRealPos.x - vTileMapLT.x && vRealPos.x - vTileMapLT.x <= TileScale.x &&
				0 <= vTileMapLT.y - vRealPos.y && vTileMapLT.y - vRealPos.y <= TileScale.y)
			{
				m_bTilemapInnerClicked = true;

				int iCol = (vRealPos.x - vTileMapLT.x) / (GetTargetObject()->TileMap()->GetTileSize().x);
				int iRow = (vTileMapLT.y - vRealPos.y) / (GetTargetObject()->TileMap()->GetTileSize().y);

				int iTileXCount = GetTargetObject()->TileMap()->GetTileCountX();
				UINT iIdx = iTileXCount * iRow + iCol;

				// test Ŭ���Ǹ��ϴ� ������ 0���̹��� �ε����� Ÿ�� ���� ** Test =
				//GetTargetObject()->TileMap()->SetTileData(iIdx, 0);
				tTileData tileData = GetTargetObject()->TileMap()->GetTileData(iIdx);
				bool bTileSet = true;
				if (-1 != tileData.iImgIdx)
					bTileSet = false;

				// Ÿ�� ���� ! ======================
				if (m_pSelected_Tile && bTileSet)
				{
					
						for (int i = 0; i < m_pSelected_Tile->iRow; ++i)
						{
							for (int k = 0; k < m_pSelected_Tile->iCol; ++k)
							{
								int TileIdx = iIdx + (iTileXCount * i) + k;
								if (TileIdx >= GetTargetObject()->TileMap()->GetTileCountX() * GetTargetObject()->TileMap()->GetTileCountY())
									continue;

								GetTargetObject()->TileMap()->SetTileData(TileIdx, m_pSelected_Tile->vTilesInfo[i * m_pSelected_Tile->iCol + k].iImgIdx);

							}
						}

						
				}
				// ======================
			}
		}


	}

	if (KEY_AWAY(KEY::LBTN))
	{
		if (m_bAutoSetNextTile && m_bTilemapInnerClicked)
		{
			m_bTilemapInnerClicked = false;
			if (m_pSelected_Tile)
			{
				// �ڵ����� ���� Ÿ�Ϸ� ��ȯ 
				wstring tileName = m_pSelected_Tile->Name;

				int infonum = _wtoi(&tileName[0]); //  "0_bsc" -> "1_bsc" -> "2_bsc" ~~~
				if (infonum + 1 < m_pSelected_Tile->_parent->num)
				{
					wstring nextTileNum = std::to_wstring(infonum + 1);
					tileName[0] = nextTileNum[0];
					m_pSelected_Tile->_parent->_parent->imgFile.find(tileName);

					map<wstring, Tile*>::iterator iter = m_pSelected_Tile->_parent->_parent->imgFile.find(tileName);

					if (iter != m_pSelected_Tile->_parent->_parent->imgFile.end())
						m_pSelected_Tile = iter->second;
				}
				else
				{
					wstring nextTileNum = std::to_wstring(0);
					tileName[0] = nextTileNum[0];

					m_pSelected_Tile->_parent->_parent->imgFile.find(tileName);

					map<wstring, Tile*>::iterator iter = m_pSelected_Tile->_parent->_parent->imgFile.find(tileName);

					if (iter != m_pSelected_Tile->_parent->_parent->imgFile.end())
						m_pSelected_Tile = iter->second;

				}
			}
		}
		
	
	}

	if (KEY_PRESSED(KEY::RBTN))
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

		if (nullptr != pCameraObj)
		{
			const Vec3 vCameraPos = pCameraObj->Transform()->GetRelativePos();
			float fWidth = pCameraObj->Camera()->GetWidth();
			float fHeight = fWidth / pCameraObj->Camera()->GetAspectRatio();

			// ���� window ȭ��â���� ���߾��� (0.f ,0.f) �� ���� �������� �� ��ǥ�� 
			Vec2 vDx11Pos = Vec2(vMousePos.x - (fWidth / 2.f), (fHeight / 2.f) - vMousePos.y);

			// ���� ����󿡼��� ��ǥ�� 
			Vec2 vRealPos = vDx11Pos + vCameraPos;

			Vec3 TilePos = GetTargetObject()->Transform()->GetRelativePos();
			Vec3 TileScale = GetTargetObject()->Transform()->GetRelativeScale();

			// Ÿ�ϸ� ��ü�� �»�� ��ǥ�� 
			Vec3 vTileMapLT = Vec3(TilePos.x - (TileScale.x / 2), TilePos.y + (TileScale.y / 2), TilePos.z);

			// Ÿ�ϸ� ���ο� Ŭ���� �־��ٸ� 
			if (0 <= vRealPos.x - vTileMapLT.x && vRealPos.x - vTileMapLT.x <= TileScale.x &&
				0 <= vTileMapLT.y - vRealPos.y && vTileMapLT.y - vRealPos.y <= TileScale.y)
			{
				int iCol = (vRealPos.x - vTileMapLT.x) / (GetTargetObject()->TileMap()->GetTileSize().x);
				int iRow = (vTileMapLT.y - vRealPos.y) / (GetTargetObject()->TileMap()->GetTileSize().y);

				int iTileXCount = GetTargetObject()->TileMap()->GetTileCountX();
				UINT iIdx = iTileXCount * iRow + iCol;
				// ����� 
				GetTargetObject()->TileMap()->SetTileData(iIdx, -1);
			}
		}
	}


}


void TileMapUI::Reset()
{
	m_TreeUI->Clear();
	
	TileImgFile* pimgFile = new TileImgFile;

	TreeNode* pFileNode = PushTileFiletoTree(L"yellowToyCastle.img", pimgFile, m_TreeUI->GetDummyNode());
	TreeNode* pNode = Push_YellowToyCastleTile_toTree(pFileNode);


}




TreeNode* TileMapUI::PushTileFiletoTree(const wstring _FileName, TileImgFile* pimgFile, TreeNode* _pDestNode)
{
	pimgFile->Name = _FileName;
	TreeNode* pNode = m_TreeUI->AddTreeNode(_pDestNode
		, string(_FileName.begin(), _FileName.end())
		, (DWORD_PTR)pimgFile);


	return pNode;
}

TreeNode* TileMapUI::PushPackageFiletoTree(TilePackage * pPack, TreeNode* _pDestNode)
{
	
	TreeNode* pNode = m_TreeUI->AddTreeNode(_pDestNode
		, string(pPack->name.begin(), pPack->name.end())
		, (DWORD_PTR)pPack);

	return pNode;
}

TreeNode* TileMapUI::PushTiletoTree(Tile* _pTile, TreeNode* _pDestNode)
{
	TreeNode* pNode = m_TreeUI->AddTreeNode(_pDestNode
		, string(_pTile->Name.begin(), _pTile->Name.end())
		, (DWORD_PTR)_pTile);
	pNode->SetObjType(OBJECT_TYPE::END);

	return pNode;

}



void TileMapUI::TileClicked(DWORD_PTR _dw)
{
	TreeNode* pNode = (TreeNode*)_dw;
	if (OBJECT_TYPE::NONE == pNode->GetObjType()) // ���̳�� ��� ���� 
		return;

	DWORD_PTR data = pNode->GetData();
	m_pSelected_Tile = (Tile*)data;

}



// Store Tile Info 
TreeNode* TileMapUI::Push_YellowToyCastleTile_toTree(TreeNode* _pDestNode)
{

	// img File
	TileImgFile* pimgFile = new TileImgFile;
	pimgFile->Name = L"yellowToyCastle";
	wstring TilemapRelativePath = L"texture//tilemap//" + pimgFile->Name + L"//";
	pimgFile->pAtlasTex = CResMgr::GetInst()->Load<CTexture>(pimgFile->Name, TilemapRelativePath + pimgFile->Name + L"Tile.png");

	// "bsc" - Tile Package
	TilePackage* pPack = new TilePackage;
	pPack->_parent = pimgFile;
	pPack->name = L"bsc";
	pPack->num = 6;
	TreeNode* PackageNode = PushPackageFiletoTree(pPack, _pDestNode);
	CreateNewTilesInfo(PackageNode, pPack, pimgFile);

	// "enH0" - Tile Package
	pPack = new TilePackage;
	pPack->_parent = pimgFile;
	pPack->name = L"enH0";
	pPack->num = 3;
	PackageNode = PushPackageFiletoTree(pPack, _pDestNode);
	CreateNewTilesInfo(PackageNode, pPack, pimgFile);

	// "enH0" - Tile Package
	pPack = new TilePackage;
	pPack->_parent = pimgFile;
	pPack->name = L"enH1";
	pPack->num = 3;
	PackageNode = PushPackageFiletoTree(pPack, _pDestNode);
	CreateNewTilesInfo(PackageNode, pPack, pimgFile);

	// "enH0" - Tile Package
	pPack = new TilePackage;
	pPack->_parent = pimgFile;
	pPack->name = L"enV0";
	pPack->num = 2;
	PackageNode = PushPackageFiletoTree(pPack, _pDestNode);
	CreateNewTilesInfo(PackageNode, pPack, pimgFile);

	// "enH0" - Tile Package
	pPack = new TilePackage;
	pPack->_parent = pimgFile;
	pPack->name = L"enV1";
	pPack->num = 2;
	PackageNode = PushPackageFiletoTree(pPack, _pDestNode);
	CreateNewTilesInfo(PackageNode, pPack, pimgFile);

	// "edDU" - Tile Package
	pPack = new TilePackage;
	pPack->_parent = pimgFile;
	pPack->name = L"edDU";
	pPack->num = 4;
	PackageNode = PushPackageFiletoTree(pPack, _pDestNode);
	CreateNewTilesInfo(PackageNode, pPack, pimgFile);

	// "sIL" - Tile Package
	pPack = new TilePackage;
	pPack->_parent = pimgFile;
	pPack->name = L"sIL";
	pPack->num = 2;
	PackageNode = PushPackageFiletoTree(pPack, _pDestNode);
	CreateNewTilesInfo(PackageNode, pPack, pimgFile);

	// "sIR" - Tile Package
	pPack = new TilePackage;
	pPack->_parent = pimgFile;
	pPack->name = L"sIR";
	pPack->num = 2;
	PackageNode = PushPackageFiletoTree(pPack, _pDestNode);
	CreateNewTilesInfo(PackageNode, pPack, pimgFile);

	return nullptr;

}

void TileMapUI::CreateNewTilesInfo(TreeNode* _pDestNode, TilePackage* _pPackage, TileImgFile* _pimgFile)
{
	if (_pPackage->name == L"bsc")
	{
		// Create bsc info
		int imgidx = 0;
		for (int i = 0; i < _pPackage->num; ++i)
		{
			Tile* pTile = new Tile;
			string number;
			pTile->Name = std::to_wstring(i) + L"_bsc";
			pTile->_parent = _pPackage;
			pTile->iImgIdxNum = 6;
			pTile->iRow = 2;
			pTile->iCol = 3;


			pTile->vTilesInfo.push_back(TileInfo(imgidx));
			pTile->vTilesInfo.push_back(TileInfo(imgidx + 1));
			pTile->vTilesInfo.push_back(TileInfo(imgidx + 2));
			pTile->vTilesInfo.push_back(TileInfo(imgidx + 9));
			pTile->vTilesInfo.push_back(TileInfo(imgidx + 10));
			pTile->vTilesInfo.push_back(TileInfo(imgidx + 11));

			_pimgFile->imgFile.insert(make_pair(pTile->Name, pTile));
			PushTiletoTree(pTile, _pDestNode);

			imgidx += 3;

			if (i == 2)
				imgidx = 18;
		}
	}

	else if (_pPackage->name == L"enH0")
	{
		// Create enH0 info
		int imgidx = 36;
		for (int i = 0; i < _pPackage->num; ++i)
		{
			Tile* pTile = new Tile;
			string number;
			pTile->Name = std::to_wstring(i) + L"_enH0";
			pTile->_parent = _pPackage;
			pTile->iImgIdxNum = 3;
			pTile->iRow = 1;
			pTile->iCol = 3;


			pTile->vTilesInfo.push_back(TileInfo(imgidx));
			pTile->vTilesInfo.push_back(TileInfo(imgidx + 1));
			pTile->vTilesInfo.push_back(TileInfo(imgidx + 2));

			_pimgFile->imgFile.insert(make_pair(pTile->Name, pTile));
			PushTiletoTree(pTile, _pDestNode);

			imgidx += 3;
		}
	}
	else if (_pPackage->name == L"enH1")
	{
		// Create enH0 info
		int imgidx = 45;
		for (int i = 0; i < _pPackage->num; ++i)
		{
			Tile* pTile = new Tile;
			string number;
			pTile->Name = std::to_wstring(i) + L"enH1";
			pTile->_parent = _pPackage;
			pTile->iImgIdxNum = 3;
			pTile->iRow = 1;
			pTile->iCol = 3;


			pTile->vTilesInfo.push_back(TileInfo(imgidx));
			pTile->vTilesInfo.push_back(TileInfo(imgidx + 1));
			pTile->vTilesInfo.push_back(TileInfo(imgidx + 2));

			_pimgFile->imgFile.insert(make_pair(pTile->Name, pTile));
			PushTiletoTree(pTile, _pDestNode);

			imgidx += 3;
		}
	}
	else if (_pPackage->name == L"enV0")
	{
		// Create enH0 info
		int imgidx = 63;
		for (int i = 0; i < _pPackage->num; ++i)
		{
			Tile* pTile = new Tile;
			string number;
			pTile->Name = std::to_wstring(i) + L"enV0";
			pTile->_parent = _pPackage;
			pTile->iImgIdxNum = 2;
			pTile->iRow = 2;
			pTile->iCol = 1;


			pTile->vTilesInfo.push_back(TileInfo(imgidx));
			pTile->vTilesInfo.push_back(TileInfo(imgidx + 9));

			_pimgFile->imgFile.insert(make_pair(pTile->Name, pTile));
			PushTiletoTree(pTile, _pDestNode);

			imgidx += 1;
		}
	}
	else if (_pPackage->name == L"enV1")
	{
		// Create enH0 info
		int imgidx = 66;
		for (int i = 0; i < _pPackage->num; ++i)
		{
			Tile* pTile = new Tile;
			string number;
			pTile->Name = std::to_wstring(i) + L"enV1";
			pTile->_parent = _pPackage;
			pTile->iImgIdxNum = 4;
			pTile->iRow = 2;
			pTile->iCol = 2;


			pTile->vTilesInfo.push_back(TileInfo(imgidx));
			pTile->vTilesInfo.push_back(TileInfo(imgidx + 1));
			pTile->vTilesInfo.push_back(TileInfo(imgidx + 9));
			pTile->vTilesInfo.push_back(TileInfo(imgidx + 10));


			_pimgFile->imgFile.insert(make_pair(pTile->Name, pTile));
			PushTiletoTree(pTile, _pDestNode);

			imgidx += 2;
		}
	}
	else if (_pPackage->name == L"edDU")
	{
	// Create enH0 info
	int imgidx = 54;
	for (int i = 0; i < _pPackage->num; ++i)
	{
		Tile* pTile = new Tile;
		string number;
		pTile->Name = std::to_wstring(i) + L"edDU";
		pTile->_parent = _pPackage;
		pTile->iImgIdxNum = 4;
		pTile->iRow = 1;
		pTile->iCol = 2;


		pTile->vTilesInfo.push_back(TileInfo(imgidx));
		pTile->vTilesInfo.push_back(TileInfo(imgidx + 1));

		_pimgFile->imgFile.insert(make_pair(pTile->Name, pTile));
		PushTiletoTree(pTile, _pDestNode);

		imgidx += 2;
	}
	}
	else if (_pPackage->name == L"sIL")
	{
	// Create enH0 info
	int imgidx = 90;
	for (int i = 0; i < _pPackage->num; ++i)
	{
		Tile* pTile = new Tile;
		string number;
		pTile->Name = std::to_wstring(i) + L"sIL";
		pTile->_parent = _pPackage;
		pTile->iImgIdxNum = 9;
		pTile->iRow = 3;
		pTile->iCol = 3;


		pTile->vTilesInfo.push_back(TileInfo(imgidx));
		pTile->vTilesInfo.push_back(TileInfo(imgidx + 1));
		pTile->vTilesInfo.push_back(TileInfo(imgidx + 2));

		pTile->vTilesInfo.push_back(TileInfo(imgidx + 9));
		pTile->vTilesInfo.push_back(TileInfo(imgidx + 9 + 1));
		pTile->vTilesInfo.push_back(TileInfo(imgidx + 9 + 2));


		pTile->vTilesInfo.push_back(TileInfo(imgidx + 18));
		pTile->vTilesInfo.push_back(TileInfo(imgidx + 18 + 1));
		pTile->vTilesInfo.push_back(TileInfo(imgidx + 18 + 2));

		_pimgFile->imgFile.insert(make_pair(pTile->Name, pTile));
		PushTiletoTree(pTile, _pDestNode);

		imgidx += 27;
	}
	}

	else if (_pPackage->name == L"sIR")
	{
	// Create enH0 info
	int imgidx = 93;
	for (int i = 0; i < _pPackage->num; ++i)
	{
		Tile* pTile = new Tile;
		string number;
		pTile->Name = std::to_wstring(i) + L"sIR";
		pTile->_parent = _pPackage;
		pTile->iImgIdxNum = 9;
		pTile->iRow = 3;
		pTile->iCol = 3;


		pTile->vTilesInfo.push_back(TileInfo(imgidx));
		pTile->vTilesInfo.push_back(TileInfo(imgidx + 1));
		pTile->vTilesInfo.push_back(TileInfo(imgidx + 2));

		pTile->vTilesInfo.push_back(TileInfo(imgidx + 9));
		pTile->vTilesInfo.push_back(TileInfo(imgidx + 9 + 1));
		pTile->vTilesInfo.push_back(TileInfo(imgidx + 9 + 2));


		pTile->vTilesInfo.push_back(TileInfo(imgidx + 18));
		pTile->vTilesInfo.push_back(TileInfo(imgidx + 18 + 1));
		pTile->vTilesInfo.push_back(TileInfo(imgidx + 18 + 2));

		_pimgFile->imgFile.insert(make_pair(pTile->Name, pTile));
		PushTiletoTree(pTile, _pDestNode);

		imgidx += 27;
	}
	}

}
