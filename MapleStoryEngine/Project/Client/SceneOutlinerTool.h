#pragma once
#include "UI.h"

#include "TreeUI.h"

/*

    ������ ������ ����ϱ� ���ؼ� 
    ���Ƿ� �����ϴ� �� 

*/
class CGameObject;
class CScene;
class CLayer;

class SceneOutlinerTool :
    public UI
{
private:
    TreeUI* m_TreeUI;

public:
    virtual void update() override;
    virtual void render_update() override;

public:
    void Reset();
    void ObjectClicked(DWORD_PTR _dw);

    void PushGameObjectToTree(CGameObject* _pObject, TreeNode* _pDestNode);
    TreeNode* PushSceneToTree(CScene* _pScene, TreeNode* _pDestNode);
    TreeNode* PushLayerToTree(CLayer* _pLayer, TreeNode* _pDestNode);


public:
    SceneOutlinerTool();
    ~SceneOutlinerTool();
};

