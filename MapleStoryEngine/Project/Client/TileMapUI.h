#pragma once
#include "ComponentUI.h"
#include <Engine/ptr.h>

/*
    ** imgFile - Tile Key **
    bsc - ����
    enH0 - ��
    enH1 - ��
    enV0 - ��
    enV1 - ��
    edU - ��/���� ���
    edD - ��/���� �ϴ�
    sILU - ���� ��� (���)
    sIRU - ���� ��� (���)
    sILD - ���� ��� (��� - Vertical Flip)
    sIRD - ���� ��� (��� - Vertical Flip)

*/
//  Ÿ�� ���� 


class TreeUI;
class CTexture;
class TreeNode;
class CGameObject;

// ============ ���� ============

struct TileInfo
{
    TileInfo(int _imgidx = -1) { iImgIdx = _imgidx; }
    int                 iImgIdx;            // �̹����ε��� ��ȣ 
};


// ============ Ÿ�� ============
// bsc, enH0 ...
struct Tile
{
    struct TileImgFile*     _parent;
    wstring                 Name;           // Ÿ�� �̹��� �̸� 
   
    int                     iImgIdxNum;    // �ϼ��� Ÿ���� �׸��µ� �ʿ��� �̹����ε��� ���� 
    int                     iRow;          // ������ � Ÿ�� 
    int                     iCol;           // ���� � Ÿ�� 

    vector<TileInfo>        vTilesInfo;

};


// ============ img ============
// img ���� 
struct TileImgFile
{
    wstring                 Name;

    Ptr<CTexture>           pAtlasTex;  // Ÿ�� �̹���
    map<wstring, Tile*>     imgFile;

};

class TileMapUI :
    public ComponentUI
{
private:
    TreeUI*                         m_TreeUI;       

    TileImgFile*                    m_pSelected_imgFile; // img File
    Tile*                           m_pSelected_Tile;

    CGameObject*                    m_pTargetObject;

    int                            m_iMapCountX;    // �� ũ�� 
    int                            m_iMapCountY;

    bool                            m_bEditMode;
    bool                            m_bShowGrid;



public:
    virtual void update() override;
    virtual void render_update() override;
    void EditorUpdate();

public:
    void Reset();
    
    // ========== �̷��� �ؾ� �ǳ�... ==================
    TreeNode* Push_YellowToyCastleTile_toTree(TreeNode* _pDestNode);


    TreeNode* PushTileFiletoTree(wstring FileName, TileImgFile* pimgFile, TreeNode* _pDestNode);
    TreeNode* PushDummyFiletoTree(wstring FIleName, TreeNode* _pDestNode);
    TreeNode* PushTiletoTree(Tile* _pTile, TreeNode* _pDestNode);

    void CreateNewTile(TreeNode* _pDestNode, TileImgFile* _pimgFile);


public:
    // delegate 
    void TileClicked(DWORD_PTR _dw);
    int LoadTexture(const wstring& _strKey, const wstring& _strRelativePath);
    

public:
    TileMapUI();
    ~TileMapUI();


};

