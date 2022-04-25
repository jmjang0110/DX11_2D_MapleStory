#include "pch.h"
#include "CStructuredBuffer.h"

#include "CDevice.h"

CStructuredBuffer::CStructuredBuffer()
	: m_desc{}
	, m_iElementSize(0)
	, m_iElementCount(0)
	, m_eType(SB_TYPE::READ_ONLY)
{
}

CStructuredBuffer::~CStructuredBuffer()
{
}

int CStructuredBuffer::Create(UINT _iElementSize, UINT _iElementCount, SB_TYPE _eType
	, bool _bCpuAccess, void* _pInitialData)
{
	m_SB = nullptr;
	m_SRV = nullptr;


	m_iElementSize = _iElementSize;
	m_iElementCount = _iElementCount;
	m_eType = _eType;

	// desc �ۼ�
	m_desc.ByteWidth = m_iElementSize * m_iElementCount; // ( ����ü ) 
	// ���� �뵵 : ID3D11Buffer -> GPU -> ShaderResourceView -> Texture Register  
	m_desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE; 
	m_desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED; // �߰� �÷��� : ����ȭ �������� �˸���. 
	m_desc.StructureByteStride = m_iElementSize; // ����ȭ ���� ��ĭ ���� ���� 

	m_desc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
	// CPUAccessFlags : GPU�� ����� ���� ������ System Memory �� ������ �� �ִ��� 
	m_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; 

	// �ʱ� Initial Data �� �ִ� ���
	if (nullptr != _pInitialData)
	{
		D3D11_SUBRESOURCE_DATA tSub = {};
		tSub.pSysMem = _pInitialData;
		DEVICE->CreateBuffer(&m_desc, &tSub, m_SB.GetAddressOf());
	}
	else
	{
		DEVICE->CreateBuffer(&m_desc, nullptr, m_SB.GetAddressOf());
	}

	// Shader Resource View ����
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};

	srvDesc.ViewDimension = D3D_SRV_DIMENSION_BUFFER;
	srvDesc.BufferEx.NumElements = m_iElementCount;

	if (FAILED(DEVICE->CreateShaderResourceView(m_SB.Get(), &srvDesc, m_SRV.GetAddressOf())))
	{
		return E_FAIL;
	}

	return S_OK;
}
