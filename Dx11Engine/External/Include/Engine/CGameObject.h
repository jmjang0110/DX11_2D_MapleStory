#pragma once
#include "CEntity.h"

#define GET_COMPONENT(type, TYPE)  class C##type* type() { return (C##type*)m_arrCom[(UINT)COMPONENT_TYPE::TYPE]; }

class CComponent;
class CTransform;
class CCollider2D;
class CAnimator2D;
class CCamera;

class CMeshRender;
class CTileMap;
class CParticleSystem;

class CScript;

class CRenderComponent;

class CGameObject :
    public CEntity
{
private:
    vector<CGameObject*>    m_vecChild;
    CComponent*             m_arrCom[(UINT)COMPONENT_TYPE::END];
    CRenderComponent*       m_pRenderComponent;

    CGameObject*            m_pParent;

    int                     m_iLayerIdx; // 게임 오브젝트 소속 레이어 인덱스

    bool                    m_bActive;
    bool                    m_bDead;

public:
    void start();
    void update();
    void lateupdate();
    void finalupdate();
    void render();

public:
    CGameObject* GetParent() { return m_pParent; }
    const vector<CGameObject*>& GetChild() { return m_vecChild; }

    // Deregister ==> 등록 취소(등록->미등록)
    // Unregister ==> 등록 안됨(등록 x == 등록->미등록, 애초에 등록된적 없음)
    void Deregister();

    void DisconnectBetweenParent();

    bool IsDead() { return m_bDead; }
    bool IsActive() { return m_bActive; }


public:
    void AddChild(CGameObject* _pChild);
    void AddComponent(CComponent* _component);
    CComponent* GetComponent(COMPONENT_TYPE _eType) { return m_arrCom[(UINT)_eType]; }
    CRenderComponent* GetRenderComponent() { return m_pRenderComponent; }

    void Destroy();

    GET_COMPONENT(Transform, TRANSFORM)
    GET_COMPONENT(MeshRender, MESHRENDER)
    GET_COMPONENT(Camera, CAMERA)
    GET_COMPONENT(Collider2D, COLLIDER2D)
    GET_COMPONENT(Animator2D, ANIMATOR2D)
    GET_COMPONENT(TileMap, TILEMAP)
    GET_COMPONENT(ParticleSystem, PARTICLESYSTEM);;

   CScript* GetScript() { return (CScript*)m_arrCom[(UINT)COMPONENT_TYPE::SCRIPT]; }

public:
    CLONE(CGameObject)

public:
    CGameObject();
    CGameObject(const CGameObject& _origin);
    ~CGameObject();

    friend class CEventMgr;
    friend class CScene;
    friend class CLayer;
};

