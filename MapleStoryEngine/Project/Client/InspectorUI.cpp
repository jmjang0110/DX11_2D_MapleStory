#include "pch.h"
#include "InspectorUI.h"

#include "TransformUI.h"
#include "MeshRenderUI.h"
#include "CameraUI.h"

#include "ScriptUI.h"
#include "MaterialUI.h"
#include "TextureUI.h"
#include "Animator2DUI.h"
#include "GraphicsShaderUI.h"
#include "ComputeShaderUI.h"
#include "MeshUI.h"
#include "ParticleSystemUI.h"
#include "Collider2DUI.h"
#include "ListUI.h"
#include "TileMapUI.h"

#include "CImGuiMgr.h"

#include <Engine/CFileMgr.h>
#include <Engine/CSceneMgr.h>
#include <Engine/CLayer.h>
#include <Engine/CResMgr.h>
#include <Engine/CKeyMgr.h>

// COMPONENT-TYPE
#include <Engine/CTransform.h>
#include <Engine/CCamera.h>
#include <Engine/CCollider2D.h>
#include <Engine/CAnimator2D.h>
#include <Engine/CMeshRender.h>
#include <Engine/CTileMap.h>
#include <Engine/CParticleSystem.h>





InspectorUI::InspectorUI()
	: UI("Inspector")
	, m_pTargetObject(nullptr)
	, m_pTargetRes(nullptr)
	, m_arrComUI{}
	, m_arrResUI{}
{
	// ComponentUI ����   
	ComponentUI* pComUI = nullptr;

	pComUI = new TransformUI;
	AddChild(pComUI);
	m_arrComUI[(UINT)COMPONENT_TYPE::TRANSFORM] = pComUI;

	pComUI = new MeshRenderUI;
	AddChild(pComUI);
	m_arrComUI[(UINT)COMPONENT_TYPE::MESHRENDER] = pComUI;

	pComUI = new CameraUI;
	AddChild(pComUI);
	m_arrComUI[(UINT)COMPONENT_TYPE::CAMERA] = pComUI;

	// Animator2D UI
	pComUI = new Animator2DUI;
	AddChild(pComUI);
	m_arrComUI[(UINT)COMPONENT_TYPE::ANIMATOR2D] = pComUI;

	// PArticle SYstem UI
	pComUI = new ParticleSystemUI;
	AddChild(pComUI);
	m_arrComUI[(UINT)COMPONENT_TYPE::PARTICLESYSTEM] = pComUI;

	//Collider2D SYstem UI
	pComUI = new Collider2DUI;
	AddChild(pComUI);
	m_arrComUI[(UINT)COMPONENT_TYPE::COLLIDER2D] = pComUI;

	// TileMapUI
	pComUI = new TileMapUI;
	AddChild(pComUI);
	m_arrComUI[(UINT)COMPONENT_TYPE::TILEMAP] = pComUI;


	// ==============
	// ResInfoUI ����
	// ==============
	ResInfoUI* pResInfoUI = nullptr;

	// MaterialUI
	pResInfoUI = new MaterialUI;
	AddChild(pResInfoUI);
	m_arrResUI[(UINT)RES_TYPE::MATERIAL] = pResInfoUI;


	// MeshUI
	pResInfoUI = new MeshUI;
	AddChild(pResInfoUI);
	m_arrResUI[(UINT)RES_TYPE::MESH] = pResInfoUI;


	// TextureUI
	pResInfoUI = new TextureUI;
	AddChild(pResInfoUI);
	m_arrResUI[(UINT)RES_TYPE::TEXTURE] = pResInfoUI;

	// GraphicsShaderUI
	pResInfoUI = new GraphicsShaderUI;
	AddChild(pResInfoUI);
	m_arrResUI[(UINT)RES_TYPE::GRAPHICS_SHADER] = pResInfoUI;

	// ComputeShaderUI
	pResInfoUI = new ComputeShaderUI;
	AddChild(pResInfoUI);
	m_arrResUI[(UINT)RES_TYPE::COMPUTE_SHADER] = pResInfoUI;



}

InspectorUI::~InspectorUI()
{
}



void InspectorUI::update()
{
	if (KEY_TAP(KEY::I))
	{
		if (IsActive())
			Deactivate();
		else
			Activate();
	}
}

void InspectorUI::render_update()
{
	GameObjectTool_SubFunc();

}



void InspectorUI::SetTargetObject(CGameObject* _pTarget)
{


	m_pTargetObject = _pTarget;

	for (int i = 0; i < (int)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrComUI[i])
		{
			// Object �� nullptr �� ���
			if (nullptr == m_pTargetObject)
				m_arrComUI[i]->Deactivate();
			else if (m_pTargetObject->GetComponent((COMPONENT_TYPE)i))
			{
				m_arrComUI[i]->Activate();
				m_arrComUI[i]->SetTargetObject(m_pTargetObject);
			}
			else
			{
				m_arrComUI[i]->Deactivate();
			}
		}
	}
	// ������Ʈ�� null �̸�
	if (nullptr == m_pTargetObject)
	{
		// ��� ��ũ��ƮUI ��Ȱ��ȭ
		for (int i = 0; i < m_vecScriptUI.size(); ++i)
		{
			m_vecScriptUI[i]->Deactivate();
		}
	}
	else
	{
		const vector<CScript*>& vecScripts = m_pTargetObject->GetScripts();
		ScriptUI* pScriptUI = nullptr;

		for (size_t i = 0; i < vecScripts.size(); ++i)
		{
			if (m_vecScriptUI.size() <= i)
				pScriptUI = AddScriptUI();
			else
				pScriptUI = m_vecScriptUI[i];

			pScriptUI->SetTargetObject(m_pTargetObject);
			pScriptUI->SetTargetScript(vecScripts[i]);
			pScriptUI->Activate();
		}

		// ScriptUI �� �� ���� ������
		if (vecScripts.size() < m_vecScriptUI.size())
		{
			// �����ϴ� UI �� ������ ������ ScriptUI ���� ��Ȱ��ȭ �Ѵ�.��
			for (int i = vecScripts.size(); i < m_vecScriptUI.size(); ++i)
			{
				m_vecScriptUI[i]->Deactivate();
			}
		}
	}


	// ResInfoUI ��Ȱ��ȭ
	for (int i = 0; i < (int)RES_TYPE::END; ++i)
	{
		if (nullptr != m_arrResUI[i] && m_arrResUI[i]->IsActive())
			m_arrResUI[i]->Deactivate();
	}
}

void InspectorUI::SetTargetResource(CRes* _pTargetRes)
{
	// ComponentUI ���� ��Ȱ��ȭ
	for (int i = 0; i < (int)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrComUI[i] && m_arrComUI[i]->IsActive())
			m_arrComUI[i]->Deactivate();
	}

	// ScriptUI ���� ��Ȱ��ȭ
	for (size_t i = 0; i < m_vecScriptUI.size(); ++i)
	{
		m_vecScriptUI[i]->Deactivate();
	}

	// Ȱ��ȭ ��ų RES_TYPE �� �˾Ƴ�
	RES_TYPE type = _pTargetRes->GetResType();

	// �ش� ���ҽ� ResInfoUI Ȱ��ȭ
	for (int i = 0; i < (int)RES_TYPE::END; ++i)
	{
		if (nullptr != m_arrResUI[(int)i])
		{
			if (i == (int)type)
			{
				m_arrResUI[i]->Activate();
				m_arrResUI[i]->SetTargetRes(_pTargetRes);
			}
			else
				m_arrResUI[i]->Deactivate();
		}
	}

}

ScriptUI* InspectorUI::AddScriptUI()
{
	ScriptUI* pScriptUI = new ScriptUI;
	pScriptUI->Deactivate();
	AddChild(pScriptUI);

	m_vecScriptUI.push_back(pScriptUI);

	return pScriptUI;
}


// ============= TOdo =====================================================
//  AddComponent ��ư���� Component ��  ������ �� �Ͼ �Լ� 
// - �⺻���� �̸� �����س��´�. 

void InspectorUI::AddComponent(DWORD_PTR _param)
{
	string strComType = (char*)_param;

	// ���� ���۳�Ʈ �̸��� ���ؼ� TargetObject�� component �� �߰�����.

	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (strComType == ToString((COMPONENT_TYPE(i))))
		{
			COMPONENT_TYPE ComType = (COMPONENT_TYPE)i;
			if (nullptr != m_pTargetObject)
			{
				switch (ComType)
				{
				case COMPONENT_TYPE::TRANSFORM:
				{
					m_pTargetObject->AddComponent(new CTransform);
				}
				break;
				case COMPONENT_TYPE::CAMERA:
				{
					m_pTargetObject->AddComponent(new CCamera);
					m_pTargetObject->Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
					m_pTargetObject->Camera()->CheckLayerMaskAll(); // iLayerMask ���° ���̾� �׸����� �˷��ش�.


				}
				break;
				case COMPONENT_TYPE::COLLIDER2D:
				{
					m_pTargetObject->AddComponent(new CCollider2D);
					m_pTargetObject->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::BOX);
					m_pTargetObject->Collider2D()->SetOffsetPos(Vec2(0.f, 0.f));
					m_pTargetObject->Collider2D()->SetOffsetScale(Vec2(100.f, 100.f));
				}
				break;

				case COMPONENT_TYPE::ANIMATOR2D:
				{
					m_pTargetObject->AddComponent(new CAnimator2D);
				}
				break;

				case COMPONENT_TYPE::MESHRENDER:
				{
					if (nullptr == m_pTargetObject->GetRenderComponent())
					{
						m_pTargetObject->AddComponent(new CMeshRender);
						m_pTargetObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
						m_pTargetObject->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"));

					}
				
				}
				break;
				case COMPONENT_TYPE::TILEMAP:
				{
					if(nullptr == m_pTargetObject->GetRenderComponent())
						m_pTargetObject->AddComponent(new CTileMap);
				}
				break;
				case COMPONENT_TYPE::PARTICLESYSTEM:
				{
					if (nullptr == m_pTargetObject->GetRenderComponent())
						m_pTargetObject->AddComponent(new CParticleSystem);
				}
				break;

				case COMPONENT_TYPE::COLLIDER3D:
				case COMPONENT_TYPE::ANIMATOR3D:
				case COMPONENT_TYPE::BOUNDINGBOX:
				case COMPONENT_TYPE::LANDSCAPE:
				case COMPONENT_TYPE::DECAL:
				case COMPONENT_TYPE::SCRIPT:
				default:
					return;

				}


				if (m_pTargetObject->GetComponent(ComType))
				{
					m_arrComUI[i]->Activate();
					m_arrComUI[i]->SetTargetObject(m_pTargetObject);
				}
			}
		}


	}



}



// =======================================================					=======================================================
// =======================================================		SUB Func	=======================================================
// =======================================================					=======================================================


void InspectorUI::GameObjectTool_SubFunc()
{

	if (nullptr != m_pTargetObject)
	{
		ImGui::BeginChild("AddComponentToTargetObject", ImVec2(200.f, 50.f), true, ImGuiWindowFlags_HorizontalScrollbar);


		if (ImGui::Button("Add Component"))
		{

			// ListUI Ȱ��ȭ�Ѵ�.
			//const map<wstring, CRes*>& mapRes = CResMgr::GetInst()->GetResList(RES_TYPE::COMPONENT);

			ListUI* pListUI = (ListUI*)CImGuiMgr::GetInst()->FindUI("##ListUI");
			pListUI->Clear();
			pListUI->SetTitle("Component List");

			for (int i = 0; i < (int)COMPONENT_TYPE::END; ++i)
			{
				if (nullptr == m_pTargetObject->GetComponent((COMPONENT_TYPE)i))
				{
					pListUI->AddList(ToString((COMPONENT_TYPE)i));

				}

			}

			pListUI->Activate();
			// TODO - ���õ� Component �� TargetObjecct �� AddComponent �Ѵ� . 
			pListUI->SetDBCEvent(this, (DBCLKED)&InspectorUI::AddComponent);

		}

		ImGui::EndChild();

	}

}

