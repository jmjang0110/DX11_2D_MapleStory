#pragma once
#include "ComponentUI.h"
#include <Engine/Ptr.h>

class CTexture;
class CAnimation2D;


enum class ANIMATOR_MODE
{
    CREATE, // Create New Frame Mode 
    MODIFY, // Modify Selected Frame Mode 
    END,

};

class Animator2DUI :
    public ComponentUI
{
private:
    ANIMATOR_MODE           m_CurMode;

    vector<string>          m_vecTexList;   // CRes ����� texture string ����
    vector<string>          m_vecAnimList;  // TargetObject�� Animation ���� 


    Ptr<CTexture>           m_pChoosenAtlas; // ���� ���õ� Texture Atlas
    
private:
    // CRATE MODE 
    // New Anim Info
    CAnimation2D*           m_pNewAnim2D;
    vector<tAnim2DFrame>	m_NewAnim_vecFrm;               // ���� ���� �ؽ�ó ���� 

    wstring                 m_NewAnim_strName;              // �ִϸ��̼� �̸� 
    tAnim2DFrame            m_NewFrameInfo;                 // �ִϸ��̼� ���� 

    Vec2                    m_NewAnim_vBackLT;              // ��׶��� ������ġ
    Vec2                    m_NewAnim_vBackSize;            // ��׶��� ������    
  
    Vec2                    m_TextureStartPos;              // Window�� Texture ���� ��ġ 
    Vec2                    m_TexSize;                      // Texture ������ 

    int                     m_FrmCnt;
    int                     m_iDemoFrameIdx;

private:
    // MODIFY MODE 
    int                     m_ModifyFrameIdx; 
    bool                    m_bModifyBtn;

    // swap
    Vec2                    m_SwapIdx;
    bool                    m_bSwapBtn;

    //Flip
    int                         m_iHorizontal_Flip;     // 1 : yes 0 : No
    int                         m_iVertical_Flip;       // 1 : yes 0 : No

public:
    virtual void update() override;
    virtual void render_update() override;


public:
    
    void TextureSelect(DWORD_PTR _param);
    void render_update_CreateAnim2DTool();
    void UpdateTool();


private:
    // main Func 
    void SelectAtlasTexture();  
    void ShowAtlasTexTool();    
    void ShowFrameTool();           

private:
    // Sub Func
    void DrawFrmaes();                          // Vector �� ����� Frame�� ��� Green box
    void DrawFrameRect();                       // Yellow / BackGround

    void DrawSettingBtn();                      // ���� ���� ������ ���� ������ư 
    void DrawShowNewFrame(ImTextureID _texid);  // ����� �ִ� ������ �ǽð� ��� 
    void DrawPlayAnimation(ImTextureID _texid); // ����� ���������� �ִϸ��̼� ���� ��� 

    void DrawAllNewFrames(ImTextureID _texid);  // ����� ������ ������ ��� 
    void DrawDecisionFrameBtn();                // Add / Delete Frame ��ư 

public:
    Animator2DUI();
    ~Animator2DUI();
};

