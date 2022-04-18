#pragma once
#include "CComponent.h"

enum class PROJ_TYPE
{
    ORTHOGRAPHIC,
    PERSPECTIVE,

};

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
    
    PROJ_TYPE           m_eProjType;


    float               m_fWidth;           // ���� ���α���  
    float               m_fAspectRatio;     // ��Ⱦ��( ���� / ���� )
    float               m_fFOV;             // Filed Of View ( �þ߰� ) 
    float               m_fFar;             // �ִ� �þ߰� 

    UINT                m_iLayerMask;
    int                 m_iCamIdx;          // RenderMgr �󿡼� 

    
public:
    void SetCameraAsMain();
    void SetWidth(float _fWidth) { m_fWidth = _fWidth; }
    void SetAspectRatio(float _fAR) { m_fAspectRatio = _fAR; }
    void SetProjType(PROJ_TYPE _eType) { m_eProjType = _eType; }
    

    float GetWidth() { return m_fWidth; }
    float GetAspectRatio() { return m_fAspectRatio; }
    PROJ_TYPE GetProjType() { return m_eProjType; }


    void CheckLayerMask(int _iLayerIdx);
    void CheckLayerMask(const wstring&& _strLayerName);
    void CheckLayerMaskAll() { m_iLayerMask = 0xffffffff; } //32�� ������ �ϰڴ�.  }


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

