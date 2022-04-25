#pragma once
#include "CRenderComponent.h"

#include "CTexture.h"

class CTileMap :
    public CRenderComponent
{
private:
    Ptr<CTexture>        m_pAtlasTex;        // �ؽ��� ����  
    Vec2                 m_vSlicePixel;      // ���ϴ� Ÿ���� ���� ���� Pixel ���� 
    Vec2                 m_vSliceUV;         // ���ϴ� Ÿ���� ���� ���� UV ���� 
  
    UINT                 m_iRowCount;
    UINT                 m_iColCount;


    //  � ¥�� Ÿ�� ������ ���� : X * Y �� Ÿ�ϸ�
    UINT                 m_iTileCountX;
    UINT                 m_iTileCountY;

    vector<tTileData>    m_vecTileData; // Ÿ�Ϻ� ������ vector �� �����Ѵ�. 
     

public:
    void SetAtlasTex(Ptr<CTexture> _pAtlasTex)
    {
        m_pAtlasTex = _pAtlasTex;
        m_vSliceUV = m_vSlicePixel / Vec2(m_pAtlasTex->Width(), m_pAtlasTex->Height());
    }

    void SetTileSize(Vec2 _vPixelSize)
    {
        m_vSlicePixel = _vPixelSize;
        if (nullptr != m_pAtlasTex)
            m_vSliceUV = m_vSlicePixel / Vec2(m_pAtlasTex->Width(), m_pAtlasTex->Height());
    }

    void SetTileMapCount(UINT _iCountX, UINT _iCountY);
    void SetTileData(int _iTileIdx, int _iImgIdx);
    void ClearTileData();


public:
    virtual void finalupdate() override;
    virtual void render() override;
    virtual void UpdateData() override;

    CLONE(CTileMap)
public:
    CTileMap();
    ~CTileMap();
};

