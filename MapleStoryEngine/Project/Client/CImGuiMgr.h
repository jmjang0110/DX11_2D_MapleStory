#pragma once
// �Լ� �����͸� typedef �� �����ߴ�. 
// �̰�� UI �� ��ü�� ��Ȯ�� �˾ƾ� �ϱ� ������ 
// ���漱���� �ϸ� �ȸ����� UI.h �� ���� include ���Ѿ��Ѵ�. 
#include "UI.h"


/*
	ImGUi �� Delgegate ���� ��Ͻ��ѳ��� 
	Progress() �� �������� �ϰ�ó�� ��Ű�� ���� 
	�������. 

*/
typedef void (UI::* PARAM_1)(DWORD_PTR);

struct tUIDelegate
{
	UI* pInst;
	PARAM_1		pFunc;
	DWORD_PTR	dwParam;
};

class CImGuiMgr
	: public CSingleton<CImGuiMgr>
{
	SINGLE(CImGuiMgr)
private:
	map<string, UI*>	m_mapUI;
	vector<tUIDelegate>	m_vecDelegate;

public:
	void init(HWND _hwnd);

	void progress();
	void render();
	void clear();

private:
	void CreateUI();

public:
	UI* FindUI(const string& _strKey);
	void AddDelegate(tUIDelegate _del) { m_vecDelegate.push_back(_del); }


};

