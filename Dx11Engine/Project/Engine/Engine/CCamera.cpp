#include "pch.h"
#include "CCamera.h"

#include "CDevice.h"

#include "CRenderMgr.h"
#include "CTransform.h"
#include "CEventMgr.h"

#include"CScene.h"
#include "CSceneMgr.h"
#include "CLayer.h"
#include "CTransform.h"


#include "CResMgr.h"
#include "CMeshRender.h"

#include "CGameObject.h"
#include "CGraphicsShader.h"

#include "CTimeMgr.h"
#include "CKeyMgr.h"




CCamera::CCamera()
	: CComponent(COMPONENT_TYPE::CAMERA)
	, m_eProjType(PROJ_TYPE::ORTHOGRAPHIC)
	, m_fWidth(0.f)
	, m_fAspectRatio(1.f)
	, m_fFOV(XM_PI / 4.f)
	, m_fFar(10000.f)
	, m_iLayerMask(0)
	, m_iCamIdx(-1) // Scene �� ������ ���� ī�޶� 
{
	m_fWidth = CDevice::GetInst()->GetRenderResolution().x;
	m_fAspectRatio = (CDevice::GetInst()->GetRenderResolution().x /
		CDevice::GetInst()->GetRenderResolution().y); // ( ��Ⱦ�� = ���� / ����  )  
	

}

CCamera::CCamera(const CCamera& _origin)
	: CComponent(_origin)
	, m_eProjType(_origin.m_eProjType)
	, m_fWidth(_origin.m_fWidth)
	, m_fAspectRatio(_origin.m_fAspectRatio)
	, m_fFOV(_origin.m_fFOV)
	, m_fFar(_origin.m_fFar)
	, m_iLayerMask(_origin.m_iLayerMask)
	, m_iCamIdx(-1)
{
}

CCamera::~CCamera()
{

}


void CCamera::SortGameObject()
{
	m_vecForward.clear();
	m_vecMasked.clear();
	m_vecOpaque.clear();

	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();

	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		// ī�޶� ���� ��� ���̾ �ƴϸ� continue
		if (!(m_iLayerMask & (1 << i)))
			continue;

		CLayer* pLayer = pCurScene->GetLayer(i);
		vector<CGameObject*>& vecObj = pLayer->GetObjects();

		for (size_t j = 0; j < vecObj.size(); ++j)
		{
			// 1. MeshRender Component �� �����ִ��� Ȯ��
			CRenderComponent* pRenderCom = vecObj[j]->GetRenderComponent();
			if (nullptr == pRenderCom)
				continue;

			if (nullptr == pRenderCom
				|| nullptr == pRenderCom->GetMesh()
				|| nullptr == pRenderCom->GetMaterial()
				|| nullptr == pRenderCom->GetMaterial()->GetShader())
			{
				continue;
			}

			// 2. SHADER_DOMAIN Ȯ�� 
			Ptr<CGraphicsShader> pShader = pRenderCom->GetMaterial()->GetShader();

			switch (pShader->GetShaderDomain())
			{
			case SHADER_DOMAIN::DOMAIN_FORWARD:
				m_vecForward.push_back(vecObj[j]);
				break;
			case SHADER_DOMAIN::DOMAIN_MASKED:
				m_vecMasked.push_back(vecObj[j]);
				break;
			case SHADER_DOMAIN::DOMAIN_OPAQUE:
				m_vecOpaque.push_back(vecObj[j]);
				break;
			}
		}
	}
}
void CCamera::render_forward()
{
	for (size_t i = 0; i < m_vecForward.size(); ++i)
	{
		m_vecForward[i]->render();
	}
}

void CCamera::render_masked()
{
	for (size_t i = 0; i < m_vecMasked.size(); ++i)
	{
		m_vecMasked[i]->render();
	}

}

void CCamera::render_opaque()
{

	for (size_t i = 0; i < m_vecOpaque.size(); ++i)
	{
		m_vecOpaque[i]->render();
	}
}



void CCamera::finalupdate()
{
	if (KEY_PRESSED(KEY::NUM1))
	{
		//m_fWidth += DT * 500.f;
		m_fFOV += DT * XM_PI / 10.f; // �þ߰� ���� 

	}

	if (KEY_PRESSED(KEY::NUM2))
	{
		//m_fWidth -= DT * 500.f;
		m_fFOV -= DT * XM_PI / 10.f; // �þ߰� ���� 

	}
	 
	// View ��� ���
	Vec3 vCamPos = Transform()->GetRelativePos();

	// view �̵����
	Matrix matViewTrans = XMMatrixTranslation(-vCamPos.x, -vCamPos.y, -vCamPos.z);
	
	// view ȸ����� 
	Matrix matViewRot = XMMatrixIdentity();

	// Right Up  Front �� �����´�, 
	Vec3 vRight = Transform()->GetWorldRightDir();
	Vec3 vUp	= Transform()->GetWorldUpDir();
	Vec3 vFront = Transform()->GetWorldFrontDir();

	// ��ġ ��� 
	matViewRot._11 = vRight.x; matViewRot._12 = vUp.x; matViewRot._13 = vFront.x;
	matViewRot._21 = vRight.y; matViewRot._22 = vUp.y; matViewRot._23 = vFront.y;
	matViewRot._31 = vRight.z; matViewRot._32 = vUp.z; matViewRot._33 = vFront.z;

	// m_matView - google : View Matrix ���¸� ��� .
	m_matView = matViewTrans * matViewRot;


	// [  ������� ���  ]

	
	
	// 1. ���� ���� 
	if (PROJ_TYPE::ORTHOGRAPHIC == m_eProjType)
	{
		float fHeight = m_fWidth / m_fAspectRatio;
		m_matProj = XMMatrixOrthographicLH(m_fWidth, fHeight, 0.f, 5000.f);

	}
	// 2. ���� ���� 
	else if (PROJ_TYPE::PERSPECTIVE == m_eProjType)
	{
		m_matProj = XMMatrixPerspectiveFovLH(m_fFOV, m_fAspectRatio, 1.f, m_fFar);

	}


	//Vec2 vRenderResolution = CDevice::GetInst()->GetRenderResolution();
	

	g_transform.matView = m_matView;
	g_transform.matProj = m_matProj;


	CRenderMgr::GetInst()->RegisterCamera(this);

}


void CCamera::SetCameraAsMain()
{
	tEventInfo tEvent = {};

	tEvent.eType = EVENT_TYPE::SET_CAMERA_INDEX;
	tEvent.lParam = (DWORD_PTR)this;
	tEvent.wParam = 0; // 0 Index  - Main Camera  

	CEventMgr::GetInst()->AddEvent(tEvent);

}

void CCamera::CheckLayerMask(int _iLayerIdx)
{
	m_iLayerMask |= 1 << _iLayerIdx;

	if (m_iLayerMask & 1 << _iLayerIdx)
	{
		m_iLayerMask &= ~(1 << _iLayerIdx);
	}

	else
	{
		m_iLayerMask |= 1 << _iLayerIdx;
	}
}

void CCamera::CheckLayerMask(const wstring&& _strLayerName)
{
	CScene* pScene = CSceneMgr::GetInst()->GetCurScene();
	CLayer* pLayer = pScene->GetLayer(_strLayerName);
	CheckLayerMask(pLayer->GetLayerIdx());

}


