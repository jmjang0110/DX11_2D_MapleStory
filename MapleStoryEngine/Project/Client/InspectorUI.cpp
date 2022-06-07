#include "pch.h"
#include "InspectorUI.h"

#include <Engine/CKeyMgr.h>

#include "TransformUI.h"
#include "MeshRenderUI.h"
#include "CameraUI.h"


#include "MaterialUI.h"
#include "TextureUI.h"
#include "Animator2DUI.h"
#include "GraphicsShaderUI.h"
#include "ComputeShaderUI.h"
#include "MeshUI.h"
#include "ParticleSystemUI.h"




#include <Engine/CFileMgr.h>
#include <Engine/CSceneMgr.h>
#include <Engine/CLayer.h>




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
	/*if (KEY_TAP(KEY::I))
	{
		if (IsActive())
			Deactivate();
		else
			Activate();
	}*/



	//ImGui::End();


}

void InspectorUI::render_update()
{
	// ���� TargetObject �� ���Ͽ� �����Ѵ�. 
	if (ImGui::Button("Save to File  "))
		CFileMgr::GetInst()->SaveToFile<CGameObject>((DWORD_PTR)m_pTargetObject);

	if (ImGui::Button("Load from File"))
	{
		CGameObject* pNewObj = (CGameObject*)CFileMgr::GetInst()->LoadFromFile<CGameObject>((DWORD_PTR)m_pTargetObject);

		CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();

		// ���° Layer�� ������ ������ ���Ѵ� 
		CLayer* pArrLayer = pCurScene->GetAllLayer();
		int LayerCnt = 0;

		for (int i = 0; i < MAX_LAYER; ++i)
		{
			if (nullptr != &pArrLayer[i])
			{
				++LayerCnt;
				ImGui::Text("%d", i);


			}
		}
	}

	// ���� TargetObject �� ���Ϸκ��� �ε��� Obj �� �ٲ۴�. 
	if (ImGui::Button("Load from File to this Object"))
	{
		CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
		if (nullptr != pCurScene)
		{
			CGameObject* pPrevTargetObj = m_pTargetObject->Clone();
			CLayer* pLayer = pCurScene->GetLayer(m_pTargetObject->GetLayerIdx());

			pLayer->DeregisterObject(m_pTargetObject);

			m_pTargetObject = (CGameObject*)CFileMgr::GetInst()->LoadFromFile<CGameObject>((DWORD_PTR)m_pTargetObject);

			if (m_pTargetObject->GetLayerIdx() < 0)
			{
				m_pTargetObject = pPrevTargetObj;
				pLayer->AddObject(m_pTargetObject);
			}
			else
			{
				SetTargetObject(m_pTargetObject);
				pLayer->AddObject(m_pTargetObject);

			}

		}
	}
	
	ImGui::Separator();
}



void InspectorUI::SetTargetObject(CGameObject* _pTarget)
{
	m_pTargetObject = _pTarget;

	for (int i = 0; i < (int)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrComUI[i])
		{
			if (m_pTargetObject->GetComponent((COMPONENT_TYPE)i))
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

	// ResInfoUI ��Ȱ��ȭ
	for (int i = 0; i < (int)RES_TYPE::END; ++i)
	{
		if (nullptr != m_arrResUI[i] && m_arrResUI[i]->IsActive())
			m_arrResUI[i]->Deactivate();
	}
}

void InspectorUI::SetTargetResource(CRes* _pTargetRes)
{

	m_pTargetRes = _pTargetRes;

	// ComponentUI ���� ��Ȱ��ȭ
	for (int i = 0; i < (int)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrComUI[i] && m_arrComUI[i]->IsActive())
			m_arrComUI[i]->Deactivate();
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