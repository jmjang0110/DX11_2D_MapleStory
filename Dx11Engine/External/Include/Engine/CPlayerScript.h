#pragma once
#include "CScript.h"


typedef struct Jumptool
{
    Vec3   _TransformPos;
    bool  _IsJumping;
    
    float _StartPosY;       // ������Ʈ�� �ʱ� ���� 
    float _Gravity;         // �߷� ���ӵ� 
    float _JumpPower;       // ������ 
    float _JumpTime;        // ���� ���� ��� �ð� 

    void start()
    {
        _TransformPos = Vec3(0.f, 0.f, 0.f);
        _IsJumping = false;
        _StartPosY = _TransformPos.y;
        _Gravity = 9.14f;
        _JumpPower = 15.0f;
        _JumpTime = 0.f;
    }



};


class CPlayerScript :
    public CScript
{
private:
    Ptr<CPrefab>    m_pMissilePrefab;
    float           m_fSpeed;

    float           m_HP;
    float           m_MP;

private:
    Jumptool        m_JumpTool;


public:
    virtual void start();
    virtual void update();
    virtual void lateupdate();

    virtual void OnCollisionEnter(CGameObject* _OtherObject) override;


    CLONE(CPlayerScript)

public:
    void Jump();


public:
    CPlayerScript();
    ~CPlayerScript();

};

