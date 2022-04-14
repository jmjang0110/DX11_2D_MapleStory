#pragma once
#include "CComponent.h"


class CGameObject;

class CCamera :
    public CComponent
{
private:
    vector<CGameObject*> m_vecForward;  // ������ ��ü 
    vector<CGameObject*> m_vecMasked;   // ���� , ������ ��ü
    vector<CGameObject*> m_vecOpaque;   // ������ ��ü 



    
    Matrix              m_matView;
    Matrix              m_matProj;


    UINT                m_iLayerMask;
    int                 m_iCamIdx; // RenderMgr �󿡼� 
public:
    void SetCameraAsMain();
    void CheckLayerMask(int _iLayerIdx);
    void CheckLayerMask(const wstring&& _strLayerName);
    void CheckLayerMaskAll() { m_iLayerMask = 0xffffffff; }


private:
    void SortGameObject(); // Shader Domain �� ���� Object �з� 


public:    
    virtual void finalupdate() override;
    void render_forward();
    void render_masked();
    void render_opaque();

    const Matrix& GetViewMat() { return m_matView; }
    const Matrix& GetProjMat() { return m_matProj; }
    CLONE(CCamera)

public:
    CCamera();
    CCamera(const CCamera& _origin);
    ~CCamera();

    friend class CRenderMgr;

};

