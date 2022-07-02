#include "pch.h"
#include "SceneOutlinerTool.h"

#include <Engine/CSceneMgr.h>
#include <Engine/CEventMgr.h>


#include <Engine/CScene.h>
#include <Engine/CLayer.h>
#include <Engine/CGameObject.h>

#include "TreeUI.h"
#include "CImGuiMgr.h"
#include "InspectorUI.h"
#include "MenuUI.h"


#include <Engine/CResMgr.h>
#include <Engine/CMesh.h>
#include <Engine/CTransform.h>
#include <Engine/CMeshRender.h>
#include <Engine/CCamera.h>


#include "CImGuiMgr.h"
#include "ListUI.h"
#include "PrefabUI.h"

#include <Script/CSceneSaveLoad.h>



SceneOutlinerTool::SceneOutlinerTool()
	: UI("SceneOutlinerTool")
	, m_pSelectedScene(nullptr)
	, m_pSelectedLayer(nullptr)
	, m_pSelectedGameObject(nullptr)
	, m_RecentClickedType(ENGINE_TYPE::NONE)
{
	m_TreeUI = new TreeUI(true);
	m_TreeUI->SetTitle("SceneOutlinerTool");
	m_TreeUI->UseFrame(false);
	m_TreeUI->ShowDummyRoot(false);

	m_TreeUI->UseDragDropSelf(true);
	m_TreeUI->UseDragDropOuter(true);

	AddChild(m_TreeUI);

	// Clicked Delegate ���
	m_TreeUI->SetClickedDelegate(this, (CLICKED)&SceneOutlinerTool::ObjectClicked);
	m_TreeUI->SetDragAndDropDelegate(this, (DRAG_DROP)&SceneOutlinerTool::DragAndDropDelegate);
	// Delete Pressed ��� 
	m_TreeUI->SetKeyBinding(KEY::DEL, this, (CLICKED)&SceneOutlinerTool::PressDelete);


	Reset();
}

SceneOutlinerTool::~SceneOutlinerTool()
{
}



void SceneOutlinerTool::update()
{

	if (CEventMgr::GetInst()->HasOccurObjEvent())
	{
		Reset();

	}
	UI::update();


}

void SceneOutlinerTool::render_update()
{
	ImGui::BeginChild("New Object", ImVec2(280.f, 50.f), true, ImGuiWindowFlags_HorizontalScrollbar);
	
	NewSceneButton();
	ImGui::SameLine();
	NewLayerButton();
	NewObjectButton();
	
	ImGui::EndChild();

	

}

void SceneOutlinerTool::Reset()
{
	m_TreeUI->Clear();


	// ���� Scene �� �����´�.
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();

	// fisrt Tree Node - Scene 
	TreeNode* pSceneNode = PushSceneToTree(pCurScene, m_TreeUI->GetDummyNode());

	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		CLayer* pLayer = pCurScene->GetLayer(i);
		if (pLayer->GetName() == L"")
			continue;

		// second Tree Node - Layer 
		TreeNode* pLayerNode = PushLayerToTree(pLayer, pSceneNode);
		
		// Third Tree Node - GameObject 
		vector<CGameObject*>& vecRoots = pLayer->GetRootObjects();
		for (size_t i = 0; i < vecRoots.size(); ++i)
		{
			PushGameObjectToTree(vecRoots[i], pLayerNode);
		}
	}
	// InspectorUI 
	InspectorUI* pInspectorUI = (InspectorUI*)CImGuiMgr::GetInst()->FindUI("Inspector");
	pInspectorUI->SetTargetObject(nullptr);

}

void SceneOutlinerTool::ObjectClicked(DWORD_PTR _dw)
{
	TreeNode* pNode = (TreeNode*)_dw;

	string strKey = pNode->GetName();
	ENGINE_TYPE _ObjType = pNode->GetObjType();
	DWORD_PTR data = pNode->GetData();
	m_RecentClickedType = _ObjType;


	switch (_ObjType)
	{
	case ENGINE_TYPE::SCENE:
	{
		m_pSelectedScene = (CScene*)data;
		m_pSelectedGameObject = nullptr;
		m_pSelectedLayer = nullptr;

		InspectorUI* pInspectorUI = (InspectorUI*)CImGuiMgr::GetInst()->FindUI("Inspector");
		pInspectorUI->SetTargetObject(nullptr);
	}
		break;
	case ENGINE_TYPE::LAYER:
	{
		m_pSelectedLayer = (CLayer*)data;
		m_pSelectedGameObject = nullptr;

		InspectorUI* pInspectorUI = (InspectorUI*)CImGuiMgr::GetInst()->FindUI("Inspector");
		pInspectorUI->SetTargetObject(nullptr);
	}
		break;
	case ENGINE_TYPE::GAME_OBJECT:
	{
		CGameObject* pObject = (CGameObject*)data;
		m_pSelectedGameObject = pObject;

		InspectorUI* pInspectorUI = (InspectorUI*)CImGuiMgr::GetInst()->FindUI("Inspector");
		pInspectorUI->SetTargetObject(pObject);
	}
		break;

	default:
		break;
	}
	

	
}

TreeNode* SceneOutlinerTool::PushSceneToTree(CScene* _pScene, TreeNode* _pDestNode)
{
	TreeNode* pNode = m_TreeUI->AddTreeNode(_pDestNode
		, "Scene - " + string(_pScene->GetName().begin(), _pScene->GetName().end())
		, (DWORD_PTR)_pScene);
	pNode->SetObjType(ENGINE_TYPE::SCENE);


	return pNode;

}

TreeNode* SceneOutlinerTool::PushLayerToTree(CLayer* _pLayer, TreeNode* _pDestNode)
{
	int layerIdx = _pLayer->GetLayerIdx();
	TreeNode* pNode = m_TreeUI->AddTreeNode(_pDestNode
		, std::to_string(layerIdx) + "Layer - " + string(_pLayer->GetName().begin(), _pLayer->GetName().end())
		, (DWORD_PTR)_pLayer);

	pNode->SetObjType(ENGINE_TYPE::LAYER);
	return pNode;

}

void SceneOutlinerTool::PushGameObjectToTree(CGameObject* _pObject, TreeNode* _pDestNode)
{
	TreeNode* pNode = m_TreeUI->AddTreeNode(_pDestNode
		, string(_pObject->GetName().begin(), _pObject->GetName().end())
		, (DWORD_PTR)_pObject);
	pNode->SetObjType(ENGINE_TYPE::GAME_OBJECT);


	const vector<CGameObject*>& vecChild = _pObject->GetChild();

	for (size_t i = 0; i < vecChild.size(); ++i)
	{
		PushGameObjectToTree(vecChild[i], pNode);
	}
}




void SceneOutlinerTool::PressDelete(DWORD_PTR _dw)
{
	TreeNode* pNode = (TreeNode*)_dw;

	if (nullptr == pNode)
		return;

	CGameObject* pTargetObj = (CGameObject*)pNode->GetData();
	pTargetObj->Destroy();

	// InspectorUI �� ã�Ƽ� Object �� nullptr �� �����Ѵ�.

	InspectorUI* pInspectorUI = (InspectorUI*)CImGuiMgr::GetInst()->FindUI("Inspector");
	pInspectorUI->SetTargetObject(nullptr);
}


void SceneOutlinerTool::DragAndDropDelegate(DWORD_PTR _dwDrag, DWORD_PTR _dwDrop)
{
	if (_dwDrag == (DWORD_PTR)nullptr || _dwDrop == (DWORD_PTR)nullptr)
		return;

	// GameObject -> GameObject 
	if (ENGINE_TYPE::GAME_OBJECT == m_TreeUI->GetDragNode()->GetObjType()
		&& ENGINE_TYPE::GAME_OBJECT == m_TreeUI->GetDropNode()->GetObjType())
	{

		CGameObject* pChildObject = (CGameObject*)_dwDrag;
		CGameObject* pDropTargetObject = (CGameObject*)_dwDrop;

		// ��� �������� ����� ��� �� ���
		if (nullptr != pDropTargetObject)
		{
			if (pChildObject == pDropTargetObject
				|| pDropTargetObject->IsAncestor(pChildObject))
			{
				return;
			}

			CSceneMgr::GetInst()->AddChild(pDropTargetObject, pChildObject);
		}

		// �ڽ� ������Ʈ�� �������� nullptr �� ���
		else
		{
			// �̹� �ֻ��� �θ� ������Ʈ�� �̺�Ʈ ó���� �ʿ����.
			if (nullptr == pChildObject->GetParent())
			{
				return;
			}

			CSceneMgr::GetInst()->DisconnectParent(pChildObject);
		}

	}

	// GameObject -> Layer 
	if (ENGINE_TYPE::GAME_OBJECT == m_TreeUI->GetDragNode()->GetObjType()
		&& ENGINE_TYPE::LAYER == m_TreeUI->GetDropNode()->GetObjType())
	{

		CGameObject* pObject = (CGameObject*)_dwDrag;
		CLayer* pDropTargetLayer = (CLayer*)_dwDrop;

		// ��� �������� ����� ��� �� ���
		if (nullptr != pDropTargetLayer)
		{
			// �ֻ��� ������Ʈ�� �ִ� ������Ʈ�� �޴´�. 
			if(pObject != pObject->GetAncestor())
				return;

			// ���� ���̾��� m_vecRoot ���� �����. 
			CSceneMgr::GetInst()->DeRegisterObjInLayer(pObject, pObject->GetLayerIndex());
			// ���ο� ���̾ ����Ѵ�.
			CSceneMgr::GetInst()->SpawnObject(pObject, pDropTargetLayer->GetLayerIdx());

		}

	}

	
}

// ====== Todo 
void SceneOutlinerTool::NewObjectButton()
{
	// Layer �� �� �� GameObject ���� ���� 
	// Create New GameObject
	if (ENGINE_TYPE::LAYER == m_RecentClickedType || ENGINE_TYPE::GAME_OBJECT == m_RecentClickedType)
	{
		if (ImGui::Button("New GameObject", ImVec2(120, 0)))
			ImGui::OpenPopup("Create New Obj");

		bool unused_open = true;
		if (ImGui::BeginPopupModal("Create New Obj", &unused_open))
		{
			ImGui::Text("Set New GameObject Name!\n\n");


			if (nullptr != m_pSelectedScene)
			{
				ImGui::Text("select Scene - ");
				ImGui::SameLine();
				string sName = string(m_pSelectedScene->GetName().begin(), m_pSelectedScene->GetName().end());
				ImGui::TextColored(ImVec4(0.0f, 0.5f, 0.5f, 1.f), sName.c_str());

			}
			else
				ImGui::TextColored(ImVec4(1.f, 0.0f, 0.0f, 1.f), "YOU MUST SELECT SCENE!");

			if (nullptr != m_pSelectedLayer)
			{
				ImGui::Text("select Layer - ");
				ImGui::SameLine();
				string sName = string(m_pSelectedLayer->GetName().begin(), m_pSelectedLayer->GetName().end());
				ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.8f, 1.f), sName.c_str());
				ImGui::Text("Add This Obj to this Layer");

			}
			else
				ImGui::TextColored(ImVec4(1.f, 0.0f, 0.0f, 1.f), "YOU MUST SELECT LAYER!");


			ImGui::Text("Show Prefab List");
			ImGui::SameLine();
			if (ImGui::Button("##PrefabListbtn", Vec2(15, 15)))
			{
				// ListUI Ȱ��ȭ�Ѵ�.
				const map<wstring, CRes*>& mapRes = CResMgr::GetInst()->GetResList(RES_TYPE::PREFAB);
				ListUI* pListUI = (ListUI*)CImGuiMgr::GetInst()->FindUI("##ListUI");
				pListUI->Clear();
				pListUI->SetTitle("Prefab List");

				for (const auto& pair : mapRes)
				{
					pListUI->AddList(string(pair.first.begin(), pair.first.end()));
				}

				pListUI->Activate();
				pListUI->SetDBCEvent(this, (DBCLKED)&::SceneOutlinerTool::PrefabSelect);
			}



			static char buf[512];
			ImGui::InputText("SetName", buf, IM_ARRAYSIZE(buf), ImGuiInputTextFlags_None);

			if (ImGui::Button("Complete"))
			{
				string name = buf;
				wstring newName = wstring(name.begin(), name.end());

				CGameObject* NewObj = new CGameObject;
				NewObj->SetName(newName);

				// Create GameObject
				if (nullptr != m_pSelectedScene && nullptr != m_pSelectedLayer)
				{
					// �⺻������ CTransform �� ���´�. 
					NewObj->AddComponent(new CTransform);

					NewObj->Transform()->SetRelativePos(0.f, 0.f, 100.f);
					NewObj->Transform()->SetRelativeScale(200.f, 200.f, 1.f);


					m_pSelectedScene->AddObject(NewObj, m_pSelectedLayer->GetName());
					// TReeUI �� �߰��ϱ� ���ؼ� Reset() 
					Reset();
					ImGui::CloseCurrentPopup();

				}


				ImGui::CloseCurrentPopup();

			}
			ImGui::EndPopup();
		}
	}

}

void SceneOutlinerTool::NewSceneButton()
{

	// ���� �������� SCene �� ������ �Ŀ� 
	// ���ο� SCene �� �����Ѵ�. 
	if (ENGINE_TYPE::NONE == m_RecentClickedType || ENGINE_TYPE::SCENE == m_RecentClickedType)
	{
		if (ImGui::Button("New Scene", ImVec2(120, 0)))
		{
			// ���� Scene�� �����Ѵ�. 
			MenuUI* pCurMenuUI = (MenuUI*)CImGuiMgr::GetInst()->FindUI("Menu");
			pCurMenuUI->SetSaveScene(true);
			ImGui::OpenPopup("Create New Scene");
		}

		bool unused_open = true;
		if (ImGui::BeginPopupModal("Create New Scene", &unused_open))
		{
			ImGui::Text("Set New Scene Name!\n\n");
			static char buf[512];
			ImGui::InputText("SetName", buf, IM_ARRAYSIZE(buf), ImGuiInputTextFlags_None);


			if (ImGui::Button("Complete"))
			{
				string name = buf;
				wstring newName = wstring(name.begin(), name.end());

				CScene* NewScene = new CScene;
				NewScene->SetName(newName);


				NewScene->SetLayerName(1, L"Default");
				
				// Camera Object �߰�
				CGameObject* pCamObj = new CGameObject;
				pCamObj->SetName(L"MainCamera");
				pCamObj->AddComponent(new CTransform);
				pCamObj->AddComponent(new CCamera);

				pCamObj->Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
				pCamObj->Camera()->SetCameraAsMain();
				pCamObj->Camera()->CheckLayerMaskAll();

				NewScene->AddObject(pCamObj, L"Default");

				CSceneMgr::GetInst()->ChangeScene(NewScene);
				// Create Scene
				Reset();
				ImGui::CloseCurrentPopup();

			}


			ImGui::EndPopup();

		}

		
	}


}

void SceneOutlinerTool::NewLayerButton()
{
	if (ENGINE_TYPE::NONE == m_RecentClickedType || ENGINE_TYPE::SCENE == m_RecentClickedType)
	{
		if (ImGui::Button("New Layer", ImVec2(120, 0)))
			ImGui::OpenPopup("Create New Layer");

		bool unused_open = true;
		if (ImGui::BeginPopupModal("Create New Layer", &unused_open))
		{
			ImGui::Text("Set New Layer Name!\n\n");
			static char buf[512];
			ImGui::InputText("SetName", buf, IM_ARRAYSIZE(buf), ImGuiInputTextFlags_None);

			ImGui::Text("Layer Idx");
			static int layerIdx = 0;
			ImGui::DragInt("##LayerIdx", &layerIdx, 1.f, 0, MAX_LAYER);

			if (ImGui::Button("Complete"))
			{
				string name = buf;
				wstring newName = wstring(name.begin(), name.end());

				CLayer* NewLayer = new CLayer;
				NewLayer->SetName(newName);

				if (nullptr != m_pSelectedScene)
				{
					bool bCheckCreate = true;

					// �ش� ���̾� �ε���/�̸�(��)�� �̹� �ִٸ� 
					if (L"" != m_pSelectedScene->GetLayer(layerIdx)->GetName()
						|| nullptr != m_pSelectedScene->GetLayer(newName))
					{
						bCheckCreate = false;
					}
				
					// Layer ���� 
					if (bCheckCreate)
					{
						m_pSelectedScene->SetLayerName(layerIdx, newName);

						Reset();
						
					}
				}
				ImGui::CloseCurrentPopup();
			}


			ImGui::EndPopup();
		}

	}
}

void SceneOutlinerTool::PrefabSelect(DWORD_PTR _param)
{
	string strSelectedName = (char*)_param;
	wstring strPrefabKey = wstring(strSelectedName.begin(), strSelectedName.end());
	wstring strContent = CPathMgr::GetInst()->GetContentPath();
	wstring FullPath = strContent + strPrefabKey;


	CPrefab* pPrefab = new CPrefab; 
	pPrefab->Load(FullPath);

	assert(pPrefab);

	if (m_pSelectedScene && m_pSelectedLayer)
	{
		// Prefab ���Ͽ� ����� gameObject �� �о �ش� Layer �� �����Ѵ�. 
		CGameObject* NewObj = pPrefab->Instantiate();
		m_pSelectedScene->AddObject(NewObj, m_pSelectedLayer->GetName());

		// TReeUI �� �߰��ϱ� ���ؼ� Reset() 
		Reset();
	}

}



