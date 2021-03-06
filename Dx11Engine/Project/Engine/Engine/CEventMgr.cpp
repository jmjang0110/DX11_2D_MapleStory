#include "pch.h"
#include "CEventMgr.h"

#include "CGameObject.h"
#include "CSceneMgr.h"
#include "CScene.h"

#include "CRenderMgr.h"



CEventMgr::CEventMgr()
{

}

CEventMgr::~CEventMgr()
{

}

void CEventMgr::update()
{
	// Dead Object 삭제
	for (size_t i = 0; i < m_vecDead.size(); ++i)
	{
		assert(m_vecDead[i]);

		// 삭제되는 오브젝트가 부모가 있다면 ( 자식 오브젝트라면 ) 
		if (m_vecDead[i]->GetParent())
		{
			m_vecDead[i]->DisconnectBetweenParent();
		}
		delete m_vecDead[i];
	}

	m_vecDead.clear();


	// Event 처리
	bool bChangeStage = false;

	for (size_t i = 0; i < m_vecEvent.size(); ++i)
	{
		switch (m_vecEvent[i].eType)
		{
		case EVENT_TYPE::CREATE_OBJ:
			// lParam : Object Adress, wParam : Layer Index
		{
			CGameObject* pObj = (CGameObject*)m_vecEvent[i].lParam;
			int iLayerIdx = (int)m_vecEvent[i].wParam;
			CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
			pCurScene->AddObject(pObj, iLayerIdx);
		}
		break;

		case EVENT_TYPE::DELETE_OBJ:
			// lParam : Object Adress
		{
			CGameObject* pDeleteObject = (CGameObject*)m_vecEvent[i].lParam;

			if (false == pDeleteObject->m_bDead)
			{
				m_vecDead.push_back(pDeleteObject);
				
				// 보유하고 있는 자식 오브젝트도 전부 Dead 체크 해둔다.
				static list<CGameObject*> queue;
				queue.clear();

				queue.push_back(pDeleteObject);
				while (!queue.empty())
				{
					CGameObject* pObj = queue.front();
					queue.pop_front();
					pObj->m_bDead = true;

					const vector<CGameObject*>& vecChild = pObj->GetChild();
					for (size_t i  = 0; i < vecChild.size(); ++i)
					{
						queue.push_back(vecChild[i]);
					}
				}
			}
		}

		break;

		case EVENT_TYPE::ADD_CHILD:
			// lParam : Parent Object, wParam : Child Object
		{
			CGameObject* pParent = (CGameObject*)m_vecEvent[i].lParam;
			CGameObject* pChild = (CGameObject*)m_vecEvent[i].wParam;

			pParent->AddChild(pChild);
		}
		break;

		case EVENT_TYPE::SET_CAMERA_INDEX:
		{
			CCamera* cam = (CCamera*)m_vecEvent[i].lParam;
			int iChangeIdx = (int)m_vecEvent[i].wParam;

			CRenderMgr::GetInst()->SwapCameraIndex(cam, iChangeIdx);

		}
		break;
		}


		// 이벤트 중에 Stage 변경 이벤트가 있었다면,
		// 나머지 이벤트는 다 무시하고 종료
		if (bChangeStage)
		{
			break;
		}
	}

	m_vecEvent.clear();
}