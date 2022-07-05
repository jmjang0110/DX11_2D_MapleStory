#pragma once
#include "CEntity.h"

class CGameObject;

class CLayer :
    public CEntity
{
private:
    vector<CGameObject*>    m_vecRoot;      // ���̾ ���� �ֻ��� �θ� ������Ʈ
    vector<CGameObject*>    m_vecObj;       // �� ������ ���� �ľǵǴ� ���̾� �Ҽ� ��� ������Ʈ
    int                     m_iLayerIdx;    // Scene ������ �ε���

// == todo ==
private:
    wstring                 m_strResKey;

public:
    void SetResKey(const wstring& _strKey) { m_strResKey = _strKey; }
    void UpdateLayerIdx(int _LayerIdx);
    void SetLayerIdx(int _LayerIdx);
    void Reset() { Safe_Del_Vec(m_vecRoot); Clear(); SetName(L""); }
// ==========
public:
    int GetLayerIdx() { return m_iLayerIdx; }


public:
    void start();
    void update();
    void lateupdate();
    void finalupdate();
   // void render();

public:
    void AddObject(CGameObject* _pObj);
    void RegisterObjectAsRoot(CGameObject* _pObj) { m_vecRoot.push_back(_pObj); }
    void RegisterObject(CGameObject* _pObj){m_vecObj.push_back(_pObj);}
    void DeregisterObject(CGameObject* _pObj);
    void Clear(){m_vecObj.clear();}

    vector<CGameObject*>& GetRootObjects() { return m_vecRoot; }
    vector<CGameObject*>& GetObjects() { return m_vecObj; }

    CLONE_DISABLE(CLayer)

public:
    CLayer();
    ~CLayer();

    friend class CSceneMgr;
    friend class CScene;
    friend class CGameObject;
    friend class CEventMgr;
};

