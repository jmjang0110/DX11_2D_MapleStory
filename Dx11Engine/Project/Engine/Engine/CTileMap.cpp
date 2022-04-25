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
	, m_iTileCountX(2)
	, m_iTileCountY(2)
{
	// �޽�, ����
	SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TileMapMtrl"));

	m_vecTileData.resize(m_iTileCountX * m_iTileCountY); // vector �� ������ �̸� �÷����´�. 


}

CTileMap::~CTileMap()
{

}



void CTileMap::finalupdate()
{
	if (nullptr == m_pAtlasTex)
		return;

	// ���� ���� pixel ���� UV ���� ��ȯ 
	// m_vSliceUV : ��ĭ �ڸ� UV ������ 
	m_vSliceUV = m_vSlicePixel / Vec2(m_pAtlasTex->Width(), m_pAtlasTex->Height());

	
	
}

void CTileMap::UpdateData()
{
	if (nullptr == m_pAtlasTex)
		return;

	GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, m_pAtlasTex);

	GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, &m_iTileCountX);
	GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_1, &m_iTileCountY);

	GetMaterial()->SetScalarParam(SCALAR_PARAM::VEC2_0, &m_vSliceUV);

	// ��� Ÿ�� �����͸� ������۸� ���� �������ͷ� ���ε�
	static CConstBuffer* pCB = CDevice::GetInst()->GetCB(CB_TYPE::TILEMAP);
	pCB->SetData(m_vecTileData.data(), sizeof(tTileData) * m_iTileCountX * m_iTileCountY);
	pCB->UpdateData(); // register binding

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

void CTileMap::SetTileData(int _iTileIdx, int _imgIdx)
{
	if (nullptr == m_pAtlasTex)
	{
		return;
	}


	m_vecTileData[_iTileIdx].iImgIdx = _imgIdx;

	// ��Ʋ�󽺿��� Ÿ���� ��, �� ���� ���ϱ�
	m_iColCount = (UINT)m_pAtlasTex->Width() / (UINT)m_vSlicePixel.x;
	m_iRowCount = (UINT)m_pAtlasTex->Height() / (UINT)m_vSlicePixel.y;

	
	// ������ Ÿ�� ������ m_vecTileData[] �� �����Ѵ�. 
	int iRow = m_vecTileData[_iTileIdx].iImgIdx / m_iColCount; // �� 
	int iCol = m_vecTileData[_iTileIdx].iImgIdx % m_iColCount; // �� 

	m_vecTileData[_iTileIdx].vLTUV = Vec2(m_vSliceUV.x * iCol, m_vSliceUV.y * iRow);
	


}

void CTileMap::ClearTileData()
{
	// m_vecTileData �����͸� ������. 
	vector<tTileData> vecTileData;
	m_vecTileData.swap(vecTileData);

	m_vecTileData.resize(m_iTileCountX * m_iTileCountY);
	

}
