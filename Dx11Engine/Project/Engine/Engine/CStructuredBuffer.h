#pragma once
#include "CEntity.h"

enum class SB_TYPE
{
    READ_ONLY,
    READ_WRITE,
};

class CStructuredBuffer :
    public CEntity
{
private:
    ComPtr<ID3D11Buffer>                m_SB;
    ComPtr<ID3D11ShaderResourceView>    m_SRV;
    ComPtr<ID3D11UnorderedAccessView>   m_UAV;

    D3D11_BUFFER_DESC                   m_desc;

    UINT                                m_iElementSize;
    UINT                                m_iElementCount;

    int                                 m_iRecentBindNumSRV; // �ֱ� ���ε� �������� ��ȣ(t)
    int                                 m_iRecentBindNumUAV; // �ֱ� ���ε� �������� ��ȣ(u)

    SB_TYPE                             m_eType;
    bool                                m_bCpuRead;

public:
    UINT GetBufferSize() { return m_iElementSize * m_iElementCount; }
    UINT GetElementCount() { return m_iElementCount; }
    UINT GetElementSize() { return m_iElementSize; }

public:
    int Create(UINT _iElementSize, UINT _iElementCount, SB_TYPE _eType, bool _bCpuAccessRead, void* _pInitialData);
    void SetData(void* _pSrc, UINT _iElementCount);

    // PIPELINE_STAGE
    void UpdateData(UINT _iStage, UINT _iRegisterNum);
    void UpdateData_CS(UINT _iRegisterNum, bool _bShaderResource);
    void Clear();

public:
    CLONE_DISABLE(CStructuredBuffer);

public:
    CStructuredBuffer();
    ~CStructuredBuffer();
};

