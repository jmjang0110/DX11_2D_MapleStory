#pragma once
#include "CScript.h"

struct JumpTool
{
    // F = 1/2 * m * v ^ 2

    float Force;            // �� �Ʒ� �� 
    float mass;             // ���� 
    float velocity;         // �ӵ�  


};


class CPlayerScript :
    public CScript
{
private:
    Ptr<CPrefab>    m_pMissilePrefab;
    float           m_fSpeed;

    float           m_HP;
    float           m_MP;
    bool            m_bJump;    // ���� ��
    JumpTool        m_sJumpTool;


public:
    virtual void start();
    virtual void update();
    virtual void lateupdate();

    virtual void OnCollisionEnter(CGameObject* _OtherObject) override;


    CLONE(CPlayerScript)

public:
    void Jump(Vec3& _vPos);


public:
    CPlayerScript();
    ~CPlayerScript();

};

