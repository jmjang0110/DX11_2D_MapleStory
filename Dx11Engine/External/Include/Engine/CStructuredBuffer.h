#pragma once
#include "CEntity.h"

enum class SB_TYPE
{
    READ_ONLY,      // : binding �� Texture ����(GPU ���� ����)�� ���̴� ���� �д´�. 
    READ_WRITE,     // : binding �� Texture ����(GPU ���� ����)�� ���̴��ڵ忡�� �������� 
};

class CStructuredBuffer :
    public CEntity
{
private:
    ComPtr<ID3D11Buffer>                m_SB; // ����ȭ ���� 
    ComPtr<ID3D11ShaderResourceView>    m_SRV;
    ComPtr<ID3D11UnorderedAccessView>   m_UAV; // : Shader ���� ������ �� �ִ� ��� UAV �� ���� �������Ϳ� ����

    D3D11_BUFFER_DESC                   m_desc;

    UINT                                m_iElementSize; // ���� ���� ( TileData ����ü ���� ) 
    UINT                                m_iElementCount;

    SB_TYPE                             m_eType;
    bool                                m_bCpuRead;

public:
    UINT GetBufferSize() { return m_iElementSize * m_iElementCount; }

public:
    int Create(UINT _iElementSize, UINT _iElementCount, SB_TYPE _eType, bool _bCpuAccessRead, void* _pInitialData);


public:
    CLONE_DISABLE(CStructuredBuffer);

public:
    CStructuredBuffer();
    ~CStructuredBuffer();
};

