#pragma once
#include "UI.h"


class MenuUI :
    public UI
{
private:
    string  m_strPlayPause;

    bool    m_bPackaging;
    bool    m_bSceneSave;
    bool    m_bSceneLoad;
    bool    m_bScenePlayPause;
    bool    m_bSceneStop;

public:
    virtual void update() override;
    virtual void render() override;
    virtual void render_update() override;

private:
    void Task();


    // ====== todo 
    //  -SceneOutlinerTool ���� Scene�� ������ �� ���� �س��� ���ؼ�
public:
    void SetSaveScene(bool _b) { m_bSceneSave = _b; }
    void SetSceneLoad(bool _b) { m_bSceneLoad = _b; }

    // ==========

public:
    MenuUI();
    ~MenuUI();
};

