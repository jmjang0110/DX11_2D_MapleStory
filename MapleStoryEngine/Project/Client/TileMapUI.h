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



struct info
{
    int                 iImgIdx;    // �̹����ε��� ��ȣ 
    Vec2                tileSize;   // px
};

// bsc, enH0 ...
struct tile
{
    struct TileImgFile*  _parent;
    wstring              Name;       // Ÿ�� �̹��� �̸� 
   
    vector<info>        vTilesInfo;
    int                 iTileNum;    // Ÿ�� ���� 
    Vec2                StartPos_px; // ��Ʋ�󽺿����� ���� ��ġ
    Vec2                AllTileSize; // UV ���� ���ϱ� ���ؼ� ��Ʋ�󽺿����� 
                                     // �ش� Ÿ���� ��ü ����� ���Ѵ�. 
};


// img ���� 
struct TileImgFile
{
    wstring                 Name;

    Ptr<CTexture>           pAtlasTex;  // Ÿ�� �̹���
    map<wstring, tile*>     imgFile;

};

class TileMapUI :
    public ComponentUI
{
private:
    TreeUI*                         m_TreeUI;       

    TileImgFile*                    m_pSelected_imgFile; // img File
    tile*                           m_pSelected_Tile;    

    CGameObject*                    m_pTargetObject;

    int                            m_iMapCountX;    // �� ũ�� 
    int                            m_iMapCountY;

public:
    virtual void update() override;
    virtual void render_update() override;

public:
    void Reset();

    // ========== �̷��� �ؾ� �ǳ�... ==================
    TreeNode* Push_darkpurpleToyCastle_toTree(TreeNode* _pDestNode);


    TreeNode* PushTileFiletoTree(wstring FileName, TileImgFile* pimgFile, TreeNode* _pDestNode);
    TreeNode* PushTileImagetoTree(tile* _pTile, TreeNode* _pDestNode);


public:
    // delegate 
    void TileClicked(DWORD_PTR _dw);
    int LoadTexture(const wstring& _strKey, const wstring& _strRelativePath);
    

public:
    TileMapUI();
    ~TileMapUI();


};

