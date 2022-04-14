#include "pch.h"
#include "CRenderMgr.h"


#include "CCamera.h"
#include "CDevice.h"

CRenderMgr::CRenderMgr()
	:m_pEditorCam(nullptr)
{

}

CRenderMgr::~CRenderMgr()
{


}

void CRenderMgr::init()
{


}

void CRenderMgr::update()
{


}

void CRenderMgr::render()
{
	if (m_vecCam.empty())
		return;

	CDevice::GetInst()->ClearTarget();

	// Main Camera �������� Render
	// [ 0 ] Camera : Main Camera  
	CCamera* pMainCam = m_vecCam[0];
	
	// Camera �� ��� Layer �� ������Ʈ���� Shader Domain �� ���� �з�ȳ�� 
	pMainCam->SortGameObject();

	g_transform.matView = pMainCam->GetViewMat();
	g_transform.matProj = pMainCam->GetProjMat();

	// Forward ��ü ������ 
	pMainCam->render_forward();

	// Masked ��ü ������ 
	pMainCam->render_masked();
	// Alpha ��ü ������ 
	pMainCam->render_opaque();


	// Sub Camera �������� Render 
	for (int i = 1; i < m_vecCam.size(); ++i)
	{
		if (nullptr == m_vecCam[i])
			continue;

		m_vecCam[i];

	}



	CDevice::GetInst()->Present();

}

void CRenderMgr::RegisterCamera(CCamera* _pCam)
{
	// ī�޶� RenderMgr �� ���� ��� �Ǵ� ��� 
	if (-1 == _pCam->m_iCamIdx)
	{
		m_vecCam.push_back(_pCam);
		int iIdx = m_vecCam.size() - 1;
		_pCam->m_iCamIdx = iIdx;
	}
	else
	{
		if (m_vecCam.size() <= _pCam->m_iCamIdx)
		{
			m_vecCam.resize((size_t)_pCam->m_iCamIdx + 1);;
		}
		
		m_vecCam[_pCam->m_iCamIdx] = _pCam;

	}
		
}

void CRenderMgr::SwapCameraIndex(CCamera* _pCam, int _iChangeIdx)
{
	for (size_t i = 0; i < m_vecCam.size(); ++i)
	{
		// ��ϵ� ī�޶� ã�Ҵٸ� 
		if (_pCam == m_vecCam[i])
		{
			if (nullptr != m_vecCam[_iChangeIdx])
			{
				m_vecCam[_iChangeIdx]->m_iCamIdx = i;
				_pCam->m_iCamIdx = _iChangeIdx;

				return;

			}
		}
	}

	assert(nullptr);


}
