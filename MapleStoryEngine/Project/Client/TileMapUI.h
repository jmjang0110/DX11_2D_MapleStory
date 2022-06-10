#pragma once
#include "ComponentUI.h"
#include <Engine/ptr.h>

class TreeUI;
class CTexture;

// Ÿ���� �ҷ� �� �� �ֵ���;; 
struct TileInfo
{
    Ptr<CTexture>       pAtlasTex;
    tTileData           tileData;

    Vec2                tileSize;

};

class TileMapUI :
    public ComponentUI
{
private:
    TreeUI* m_TreeUI; // �� Ÿ�ϸ��� Atlas ������ ����.

    UINT                m_iTileCountX;
    UINT                m_iTileCountY;


public:
    virtual void update() override;
    virtual void render_update() override;


public:
    TileMapUI();
    ~TileMapUI();


};

