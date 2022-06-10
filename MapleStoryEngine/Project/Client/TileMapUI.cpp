#include "pch.h"
#include "TileMapUI.h"

#include "TreeUI.h"


#include <Engine/CTileMap.h>

/*
	
	* ������ ���丮 Ÿ���� ����Ŷ� �� ĭ�� 90x60 ���� ������̴�. 
	
	* Ʈ�� �� ����ؼ� Ÿ���� �� �� �ֵ��� �����
	
	* tile info �� �������� �����ؼ� Ÿ���� ���� �� Ÿ���� �̹����� ����
	  ������ ��� ��
	 
	 * �̹����� ���� ���ؼ� �̸� Ÿ�� ����� ������ ���� �� 
	


*/

TileMapUI::TileMapUI()
	: ComponentUI("TileMap", COMPONENT_TYPE::TILEMAP)
	, m_TreeUI(nullptr)
	, m_iTileCountX(0)
	, m_iTileCountY(0)

{
	SetSize(Vec2(0.f, 300.f));



}

TileMapUI::~TileMapUI()
{

}

void TileMapUI::update()
{
	ComponentUI::update();





}

void TileMapUI::render_update()
{
	ComponentUI::render_update();


	CGameObject* pObject = GetTargetObject();

	int TileCountX = pObject->TileMap()->GetTileCountX();
	int TileCountY = pObject->TileMap()->GetTileCountY();

	ImGui::PushItemWidth(100);

	ImGui::Text("Tile Count");
	ImGui::SameLine(150);
	ImGui::InputInt("##TileCountX", &TileCountX);
	ImGui::SameLine();
	ImGui::InputInt("##TileCountY", &TileCountY);

	pObject->TileMap()->SetTileMapCount(TileCountX, TileCountY);

	// �̰� ����� �̹����� ��µ� 
	for (int i = 0; i < TileCountX * TileCountY; ++i)
	{
		pObject->TileMap()->SetTileData(i, 0);
	}


	ImGui::PopItemWidth();

	ImGui::PushItemWidth(200);

	Vec2 SlicePixel = pObject->TileMap()->GetTileSize();
	ImGui::Text("Tile px Size");
	ImGui::SameLine(150);
	ImGui::InputFloat2("##TilepxSize", &SlicePixel.x);
	pObject->TileMap()->SetTileSize(SlicePixel);

	ImGui::PopItemWidth();



	

}

