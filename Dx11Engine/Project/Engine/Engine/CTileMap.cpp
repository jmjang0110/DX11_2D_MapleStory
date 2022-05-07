#include "pch.h"
#include "CTileMap.h"

#include "CTransform.h"

#include "CResMgr.h"

#include "CDevice.h"
#include "CConstBuffer.h"

CTileMap::CTileMap()
	: CRenderComponent(COMPONENT_TYPE::TILEMAP)
	, m_iRowCount(0)
	, m_iColCount(0)
	, m_iTileCountX(0)
	, m_iTileCountY(0)
	, m_bBufferUpdated(false)
{
	// �޽�, ����
	SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TileMapMtrl"));

	m_vecTileData.resize((size_t)(m_iTileCountX * m_iTileCountY));
	m_pBuffer = new CStructuredBuffer;
}

CTileMap::CTileMap(const CTileMap& _origin)
	: CRenderComponent(_origin)
	, m_vSlicePixel(_origin.m_vSlicePixel)
	, m_vSliceUV(_origin.m_vSliceUV)
	, m_iRowCount(_origin.m_iRowCount)
	, m_iColCount(_origin.m_iColCount)
	, m_iTileCountX(_origin.m_iTileCountX)
	, m_iTileCountY(_origin.m_iTileCountY)
	, m_vecTileData(_origin.m_vecTileData)
	, m_pBuffer(nullptr)
	, m_bBufferUpdated(_origin.m_bBufferUpdated)
{
	m_pBuffer = new CStructuredBuffer;
}

CTileMap::~CTileMap()
{
	SAFE_DELETE(m_pBuffer);
}

void CTileMap::finalupdate()
{

}

void CTileMap::UpdateData()
{
	if (nullptr == m_pAtlasTex)
		return;

	GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, m_pAtlasTex);

	GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, &m_iTileCountX);
	GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_1, &m_iTileCountY);

	GetMaterial()->SetScalarParam(SCALAR_PARAM::VEC2_0, &m_vSliceUV);


	if (false == m_bBufferUpdated)
	{
		// ��� Ÿ�� ������(m_vecTileData) �� ����ȭ���۸� ���� t16 �������ͷ� ���ε�
		m_pBuffer->SetData(m_vecTileData.data(), m_iTileCountX * m_iTileCountY);
		m_pBuffer->UpdateData(PIPELINE_STAGE::PS, 16);
		m_bBufferUpdated = true;

	}

	//pCB->SetData(m_vecTileData.data(), sizeof(tTileData) * m_iTileCountX * m_iTileCountY);
	//pCB->UpdateData();
}

void CTileMap::render()
{
	if (nullptr == GetMesh() || nullptr == GetMaterial() || nullptr == m_pAtlasTex)
		return;

	UpdateData();

	Transform()->UpdateData();
	GetMaterial()->UpdateData();
	GetMesh()->render();
}


void CTileMap::SetTileMapCount(UINT _iCountX, UINT _iCountY)
{
	m_iTileCountX = _iCountX;
	m_iTileCountY = _iCountY;

	ClearTileData();
}

void CTileMap::SetTileData(int _iTileIdx, int _iImgIdx)
{
	if (nullptr == m_pAtlasTex)
	{
		return;
	}

	m_vecTileData[_iTileIdx].iImgIdx = _iImgIdx;

	// ��Ʋ�󽺿��� Ÿ���� ��, �� ���� ���ϱ�
	m_iColCount = (UINT)m_pAtlasTex->Width() / (UINT)m_vSlicePixel.x;
	m_iRowCount = (UINT)m_pAtlasTex->Height() / (UINT)m_vSlicePixel.y;

	int iRow = m_vecTileData[_iTileIdx].iImgIdx / m_iColCount;
	int iCol = m_vecTileData[_iTileIdx].iImgIdx % m_iColCount;

	m_vecTileData[_iTileIdx].vLTUV = Vec2(m_vSliceUV.x * iCol, m_vSliceUV.y * iRow);
	m_bBufferUpdated = false;


}

void CTileMap::ClearTileData()
{
	vector<tTileData> vecTileData;
	m_vecTileData.swap(vecTileData);

	m_vecTileData.resize((size_t)(m_iTileCountX * m_iTileCountY));

	// ����ȭ ���۵� ũ�⿡ �����Ѵ�.
	size_t iBufferSize = (m_iTileCountX * m_iTileCountY) * sizeof(tTileData);

	if (m_pBuffer->GetBufferSize() < iBufferSize)
	{
		m_pBuffer->Create(sizeof(tTileData), m_iTileCountX * m_iTileCountY, SB_TYPE::READ_ONLY, false, nullptr);
	}
}