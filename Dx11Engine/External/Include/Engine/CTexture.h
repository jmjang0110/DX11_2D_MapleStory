#pragma once
#include "CRes.h"

#include <DirectXTex/DirectXTex.h>

#ifdef _DEBUG
#pragma comment(lib, "DirectXTex/DirectXTex_debug")
#else
#pragma comment(lib, "DirectXTex/DirectXTex")
#endif


class CTexture :
    public CRes
{
private:
    ScratchImage                        m_Image;    // ���Ϸκ��� �ε��� �ؽ��� �������� �ý��� �޸� ����
    ComPtr<ID3D11Texture2D>             m_pTex2D;   // GPU �� �ε��� �ؽ��� �����͸� �����ϴ� �������̽�
    ComPtr<ID3D11ShaderResourceView>    m_pSRV;     // Texture2D �� ShaderResource �뵵�� �����ϴ� �߰� �Ű�ü
    D3D11_TEXTURE2D_DESC                m_tDesc;


protected:
    virtual int Load(const wstring& _strFilePath);


public:
    float Width() { return (float)m_tDesc.Width; }
    float Height() { return (float)m_tDesc.Height; }


public:
    // PIPELINE_STAGE
    void UpdateData(UINT _PipelineStage, int _iRegisterNum);
    static void Clear(int _iRegisterNum);

public:
    CTexture();
    ~CTexture();
};

