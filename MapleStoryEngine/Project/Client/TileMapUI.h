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
struct Tile
{
    struct TilePackage*     _parent;
    wstring                 Name;            // Ÿ�� �̹��� �̸� 
   
    int                     iImgIdxNum;      // �ϼ��� Ÿ���� �׸��µ� �ʿ��� �̹����ε��� ���� 
    int                     iRow;            // ������ � Ÿ�� 
    int                     iCol;            // ���� � Ÿ�� 

    vector<TileInfo>        vTilesInfo;

};

// ============ ��Ű�� ==========
struct TilePackage
{
    struct TileImgFile* _parent;

    wstring name;                           // ��Ű�� �̸� 
    int num;                                // ��Ű���� ���� �ִ� Ÿ�� ���� 
};

// ============ img ============
// img ���� 
struct TileImgFile
{
    wstring                 Name;

    Ptr<CTexture>           pAtlasTex;      // Ÿ�� �̹���
    map<wstring, Tile*>     imgFile;


public:
    TileImgFile() {};
    ~TileImgFile() {

        Safe_Del_Map(imgFile);
    }
};




class TileMapUI :
    public ComponentUI
{
private:
    // ������ ���� img
    TreeUI*                         m_TreeUI;       
    vector<TreeNode*>               m_vimgNode;

    TileImgFile*                    m_pSelected_imgFile; // img File
    string                          m_Selected_imgFIle_Name;
    Tile*                           m_pSelected_Tile;


    int                             m_iMapCountX;    // �� ũ�� 
    int                             m_iMapCountY;

    Vec2                            m_vTileSize;


    // Button 
    bool                            m_bEditMode;
    bool                            m_bAutoSetNextTile;
    bool                            m_bUseMapleStory_ImgFile;
    bool                            m_bimgFIleChange;
    int                             m_bTilemapInnerClicked;


private:
    // �Ϲ� Img 
    TileImgFile                     m_ImgFile; // ������ ���丮 imgFile �� ���� ���� �� ���� img 
    TilePackage*                    m_tilePackage;
    Tile*                           m_tile;
    bool                            m_bUseThisTile;

    int                             m_iRow;
    int                             m_iCol;

 
private:
    // ����� TileImgFile , Package ��ϵ��� �����س��´�. - ������ �� ���� ���ؼ�
    // m_pTreeUI �� ����� TreeNode ���� �������� Node �ȿ� �ִ� 
    // �����͵��� ������ �ʴ´�. 
    // �׷��� ���� �����͵��� �����س��� ������̴�. 
    vector<TileImgFile*>            m_StoreimgFile;
    vector<TilePackage*>            m_StorePackage;

public:
    virtual void update() override;
    virtual void render_update() override;


    void EditorUpdate();
    void SelectAtlasTexture();
    void SelectTile();

public:
    void Reset();
    
    // ========== �̷��� �ؾ� �ǳ�... ==================
    TreeNode* FillimgFIleinfo(TreeNode* _pDestNode, TileImgFile* pimgFile);


    TreeNode* PushimgFiletoTree(wstring FileName, TreeNode* _pDestNode);                         //  img
    TreeNode* PushPackageFiletoTree(TilePackage* pPack, TreeNode* _pDestNode);                   //  pack
    TreeNode* PushTiletoTree(Tile* _pTile, TreeNode* _pDestNode);                                //  Tile
     
    void CreateNewTilesInfo(TreeNode* _pDestNode, TilePackage* _pPackage, TileImgFile* _pimgFile);                       //  info 


private:
    Tile* FillTileInfo(int _TileID_Number,wstring PackageName, TilePackage* _pPackage
        , int _startImgIdx, int _iAllImgIdxNum, int _row, int _col);
    void ResetimgFile(string _imgFileName, TreeNode* _imgFIleNode);

public:
    // delegate 
    void TileClicked(DWORD_PTR _dw);
  

public:
    TileMapUI();
    ~TileMapUI();


};

